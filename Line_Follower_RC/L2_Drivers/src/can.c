/*
 *     SocialLedge.com - Copyright (C) 2013
 *
 *     This file is part of free software framework for embedded processors.
 *     You can use it and/or distribute it as long as this copyright header
 *     remains unmodified.  The code is free for personal use and requires
 *     permission to use in a commercial product.
 *
 *      THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 *      OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 *      MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 *      I SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 *      CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *     You can reach the author of this software at :
 *          p r e e t . w i k i @ g m a i l . c o m
 */
#include <string.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "can.h"
#include "LPC17xx.h"
#include "sys_config.h"
#include "lpc_sys.h"    // sys_get_uptime_ms()



/// If non-zero, test code is enabled, and each message sent is self-recepted
#define CAN_TESTING         0

/// CAN index is one less than what HW numbers it
#define CAN_INDEX(x)         (x-1)
#define CAN_VALID(x)         (x > can_inv && x < can_max)
#define CAN_ENUM_TO_REGS(x)  (can1 == x) ? LPC_CAN1 : (can2 == x) ? LPC_CAN2 : NULL

// Used by CAN_CT_ASSERT().  Obtained from http://www.pixelbeat.org/programming/gcc/static_assert.html
#define CAN_ASSERT_CONCAT_(a, b) a##b
#define CAN_ASSERT_CONCAT(a, b) CAN_ASSERT_CONCAT_(a, b)
#define CAN_CT_ASSERT(e) enum { CAN_ASSERT_CONCAT(assert_line_, __LINE__) = 1/(!!(e)) }

// Make some compile-time (CT) checks :
// Check the sizes of the structs because the size needs to match the HW registers
CAN_CT_ASSERT( 2 == sizeof(can_std_id_t));
CAN_CT_ASSERT( 4 == sizeof(can_ext_id_t));
CAN_CT_ASSERT( 8 == sizeof(can_data_t));
CAN_CT_ASSERT(16 == sizeof(can_msg_t));
CAN_CT_ASSERT(12 == sizeof(can_fullcan_msg_t));



/// Interrupt masks of the CANxIER and CANxICR registers
enum {
    intr_rx   = (1 << 0),   ///< Receive
    intr_tx1  = (1 << 1),   ///< Transmit 1
    intr_warn = (1 << 2),   ///< Warning
    intr_ovrn = (1 << 3),   ///< Data overrun
    intr_wkup = (1 << 4),   ///< Wake-up
    intr_epi  = (1 << 5),   ///< Error passive
    intr_ali  = (1 << 6),   ///< Arbitration lost
    intr_berr = (1 << 7),   ///< Bus error
    intr_idi  = (1 << 8),   ///< ID ready
    intr_tx2  = (1 << 9),   ///< Transmit 2
    intr_tx3  = (1 << 10),  ///< Transmit 3
    intr_all_tx = (intr_tx1 | intr_tx2 | intr_tx3),
};

/// Bit mask of SR register indicating which hardware buffer is available
enum {
    tx1_avail = (1 << 2),
    tx2_avail = (1 << 10),
    tx3_avail = (1 << 18),
    tx_all_avail = (tx1_avail | tx2_avail | tx3_avail),
};

/**
 * Data values of the AFMR register
 * @note Since AFMR is common to both controllers, when bypass mode is enabled,
 *       then ALL messages from ALL CAN controllers will be accepted
 */
enum {
    afmr_enabled   = 0x00, ///< Hardware acceptance filtering
    afmr_disabled  = 0x01, ///< No messages will be accepted
    afmr_bypass    = 0x02, ///< Bypass mode, all messages will be accepted.  Both 0x02 or 0x03 will work.
    afmr_fullcan   = 0x04, ///< Hardware will receive and store messages per FullCAN mode.
};

/// CAN MOD register values
enum {
    can_mod_normal = 0x00, ///< CAN MOD register value to enable the BUS
    can_mod_reset  = 0x01, ///< CAN MOD register value to reset the BUS
};

/// Mask of PCONP register
enum {
    can1_pconp_mask = (1 << 13),
    can2_pconp_mask = (1 << 14),
};

/// CAN interrupt call-backs for the user
typedef struct {
    can_void_func_t bus_error;      ///< When serious BUS error occurs
    can_void_func_t data_overrun;   ///< When we read the CAN buffer too late for incoming message
} can_callbacks_t;

static xQueueHandle m_can_rx_qs[can_max-1] = { NULL, NULL };   ///< RX queue handles for CAN BUSes
static xQueueHandle m_can_tx_qs[can_max-1] = { NULL, NULL };   ///< TX queue handles for CAN BUSes
static can_callbacks_t m_can_callbacks[can_max-1]  = { {NULL, NULL}, {NULL, NULL} }; ///< Callback functions
/** @{ Private functions */



/**
 * Sends a message using one of the three available buffers
 * If all buffers are used, then false is returned.
 * @warning This should be called from critical section since this method is not thread-safe
 */
static bool CAN_tx_now (volatile LPC_CAN_TypeDef *CANx, can_msg_t *pCanMsg)
{
    // 32-bit command of CMR register to start transmission of one of the buffers
    enum {
        go_cmd_inv = 0,
        go_cmd_tx1 = 0x21,
        go_cmd_tx2 = 0x41,
        go_cmd_tx3 = 0x81,
    };

    const uint32_t can_sr_reg = CANx->SR;
    volatile uint32_t *pTxBuffer = NULL;
    uint32_t go_cmd = go_cmd_inv;

    if (can_sr_reg & tx1_avail){
        pTxBuffer = &(CANx->TFI1);
        go_cmd = go_cmd_tx1;
    }
    else if (can_sr_reg & tx2_avail){
        pTxBuffer = &(CANx->TFI2);
        go_cmd = go_cmd_tx2;
    }
    else if (can_sr_reg & tx3_avail){
        pTxBuffer = &(CANx->TFI3);
        go_cmd = go_cmd_tx3;
    }
    else {
        /* No buffer available, return failure */
        return false;
    }

    /* Tx buffer points to one of the three buffers on HW
     * Copy the data structure from SW to HW registers
     */
    volatile can_msg_t *pHwMsgRegs = (can_msg_t*)pTxBuffer;
    *pHwMsgRegs = *pCanMsg;

    #if CAN_TESTING
    go_cmd &= (0xF0);
    go_cmd |= (1 << 4); /* Self reception */
    #endif

    /* Send the message! */
    CANx->CMR = go_cmd;
    return true;
}

static inline void CAN_rx_now(volatile LPC_CAN_TypeDef *CANx, can_msg_t *pMsg)
{
    const uint32_t rx_avail_mask = (1 << 0);
    volatile can_msg_t *pMsgAtHwRegs = (can_msg_t*) &(CANx->RFS);

    /**
     * Check if a message is available and read all of the structure.  It is cheaper to read
     * all of the data structure even if message is an RTR, or if message data length is less
     * than 8 bytes because the conditional logic would be slower than reading all 16 msg bytes
     */
    if (CANx->GSR & rx_avail_mask) {
        *pMsg = *pMsgAtHwRegs;  ///< Read all bytes of the message
        CANx->CMR = 0x04;       ///< Release the receive buffer

        // zero-out invalid message id bits
        pMsg->msg_id &= (pMsg->frame_fields.is_29bit) ? 0x1FFFFFFF : 0x7FF;
    }
}

static void CAN_handle_isr(const uint32_t ibits, const can_t can, volatile LPC_CAN_TypeDef *CANx)
{
    const uint8_t cidx = CAN_INDEX(can);
    can_msg_t msg;

    /* Handle received message */
    if (ibits & intr_rx) {
        /**
         * TO DO Test if we directly have FreeRTOS queue copy the data from CAN registers.
         * can_msg_t *pMsg = (can_msg_t*) &(CANx->RFS);
         * xQueueSendFromISR(m_can_rx_qs[CAN_INDEX(can)], pMsg, NULL);
         * CANx->CMR = 0x04;
         */
        CAN_rx_now(CANx, &msg);
        xQueueSendFromISR(m_can_rx_qs[cidx], &msg, NULL);
    }

    /* A transmit finished, send any queued message(s) */
    if (ibits & intr_all_tx) {
        if (xQueueReceiveFromISR(m_can_tx_qs[cidx], &msg, NULL)) {
            CAN_tx_now(CANx, &msg);
        }
    }

    /* We only enable interrupt when a valid callback exists, so no need
     * to check for the callback function being NULL
     */
    if (ibits & intr_berr) {
        m_can_callbacks[cidx].bus_error();
    }
    if (ibits & intr_ovrn) {
        m_can_callbacks[cidx].data_overrun();
    }
}
/** @} */

/**
 * Actual ISR Handler (mapped to startup file's interrupt vector function name)
 * This interrupt is shared between CAN1, and CAN2
 */
#ifdef __cplusplus
extern "C" {
#endif
void CAN_IRQHandler(void)
{
    const uint32_t pconp = LPC_SC->PCONP;

    /* Reading registers without CAN powered up will cause DABORT exception */
    if (pconp & can1_pconp_mask) {
        const uint32_t c1_ibits = LPC_CAN1->ICR;
        if (c1_ibits) {
            CAN_handle_isr(c1_ibits, can1, LPC_CAN1);
        }
    }

    if (pconp & can2_pconp_mask) {
        const uint32_t c2_ibits = LPC_CAN2->ICR;
        if (c2_ibits) {
            CAN_handle_isr(c2_ibits, can2, LPC_CAN2);
        }
    }
}
#ifdef __cplusplus
}
#endif



bool CAN_init(can_t can, uint32_t baudrate_kbps, uint16_t rxq_size, uint16_t txq_size,
              can_void_func_t bus_off_cb, can_void_func_t data_ovr_cb)
{
    const uint8_t cidx = CAN_INDEX(can);
    volatile LPC_CAN_TypeDef *CANx = CAN_ENUM_TO_REGS(can);
    bool failed = true;

    if (!CANx){
        return false;
    }

    /* Enable CAN Power, and select the PINS
     * CAN1 is at P0.0, P0.1 and P0.21, P0.22
     * CAN2 is at P0.4, P0.5 and P2.7,  P2.8
     * On SJ-One board, we have P0.0, P0.1, and P2.7, P2.8
     */
    if (can1 == can) {
        LPC_SC->PCONP |= can1_pconp_mask;
        LPC_PINCON->PINSEL0 &= ~(0xF << 0);
        LPC_PINCON->PINSEL0 |=  (0x5 << 0);
    }
    else if (can2 == can){
        LPC_SC->PCONP |= can2_pconp_mask;
        LPC_PINCON->PINSEL4 &= ~(0xF << 14);
        LPC_PINCON->PINSEL4 |=  (0x5 << 14);
    }

    /* Create the queues with minimum size of 1 to avoid NULL pointer reference */
    if (!m_can_rx_qs[cidx]) {
        m_can_rx_qs[cidx] = xQueueCreate(rxq_size ? rxq_size : 1, sizeof(can_msg_t));
    }
    if (!m_can_tx_qs[cidx]) {
        m_can_tx_qs[cidx] = xQueueCreate(txq_size ? txq_size : 1, sizeof(can_msg_t));
    }

    /* The CAN dividers must all be the same for both CANs
     * Set the dividers of CAN1, CAN2, ACF to CLK / 1
     */
    lpc_pclk(pclk_can1, clkdiv_1);
    lpc_pclk(pclk_can2, clkdiv_1);

    CANx->MOD = can_mod_reset;
    CANx->IER = 0x0; // Disable All CAN Interrupts
    CANx->GSR = 0x0; // Clear error counters
    CANx->CMR = 0xE; // Abort Tx, release Rx, clear data over-run

    /**
     * About the AFMR register :
     *                      B0              B1
     * Filter Mode |    AccOff bit  |   AccBP bit   |   CAN Rx interrupt
     * Off Mode             1               0           No messages accepted
     * Bypass Mode          X               1           All messages accepted
     * FullCAN              0               0           HW acceptance filtering
     */
    LPC_CANAF->AFMR = afmr_disabled;

    // Clear pending interrupts and the CAN Filter RAM
    LPC_CANAF_RAM->mask[0] = CANx->ICR;
    memset((void*)&(LPC_CANAF_RAM->mask[0]), 0, sizeof(LPC_CANAF_RAM->mask));

    /* Zero out the filtering registers */
    LPC_CANAF->SFF_sa     = 0;
    LPC_CANAF->SFF_GRP_sa = 0;
    LPC_CANAF->EFF_sa     = 0;
    LPC_CANAF->EFF_GRP_sa = 0;
    LPC_CANAF->ENDofTable = 0;

    /* Enable bypass mode */
    LPC_CANAF->AFMR = afmr_bypass;

    /* Set the baud-rate. You can verify the settings by visiting:
     * http://www.kvaser.com/en/support/bit-timing-calculator.html
     */
    do {
        const uint32_t baudDiv = sys_get_cpu_clock() / (1000 * baudrate_kbps);
        const uint32_t SJW = 3;
        const uint32_t SAM = 0;
        uint32_t BRP = 0, TSEG1 = 0, TSEG2 = 0, NT = 0;

        /* Calculate suitable nominal time value
         * NT (nominal time) = (TSEG1 + TSEG2 + 3)
         * NT <= 24
         * TSEG1 >= 2*TSEG2
         */
        failed = true;
        for(NT=24; NT > 0; NT-=2) {
            if ((baudDiv % NT)==0) {
                BRP = baudDiv / NT - 1;
                NT--;
                TSEG2 = (NT/3) - 1;
                TSEG1 = NT -(NT/3) - 1;
                failed = false;
                break;
            }
        }

        if (!failed) {
            CANx->MOD = can_mod_reset;
            CANx->BTR  = (SAM << 23) | (TSEG2<<20) | (TSEG1<<16) | (SJW<<14) | BRP;
            // CANx->BTR = 0x002B001D; // 48Mhz 100Khz
        }
    } while (0);

    /* If everything okay so far, enable the CAN interrupts */
    if (!failed) {
        /* At minimum, we need Rx/Tx interrupts */
        CANx->IER = (intr_rx | intr_all_tx);

        /* Enable BUS-off interrupt and callback if given */
        if (bus_off_cb) {
            m_can_callbacks[cidx].bus_error = bus_off_cb;
            CANx->IER |= intr_berr;
        }
        /* Enable data-overrun interrupt and callback if given */
        if (data_ovr_cb) {
            m_can_callbacks[cidx].data_overrun = data_ovr_cb;
            CANx->IER |= intr_ovrn;
        }

        /* Finally, enable the actual CPU core interrupt */
        NVIC_EnableIRQ(CAN_IRQn);
    }

    /* return true if all is well */
    return (false == failed);
}


bool CAN_tx (can_t can, can_msg_t *pCanMsg, uint32_t timeout_ms)
{
    volatile LPC_CAN_TypeDef *CANx = CAN_ENUM_TO_REGS(can);
    const uint8_t cidx = CAN_INDEX(can);
    bool ok = false;

    if (!CANx || !pCanMsg) {
        return false;
    }

    /* Try transmitting to one of the available buffers */
    vPortEnterCritical();
    {
        ok = CAN_tx_now(CANx, pCanMsg);
    }
    vPortExitCritical();

    /* If all three buffers are busy, just queue the message */
    if (!ok) {
        if (taskSCHEDULER_RUNNING == xTaskGetSchedulerState()) {
            ok = xQueueSend(m_can_tx_qs[cidx], pCanMsg, OS_MS(timeout_ms));
        }
        else {
            ok = xQueueSendFromISR(m_can_tx_qs[cidx], pCanMsg, NULL);
        }

        /* There is possibility that before we queued the message, we got interrupted
         * and all hw buffers were emptied meanwhile, and our queued message will now
         * sit in the queue forever until another Tx interrupt takes place.
         * So we dequeue it here if all are empty and send it over.
         */
        vPortEnterCritical();
        {
            can_msg_t msg;
            if (tx_all_avail == (CANx->SR & tx_all_avail) &&
                xQueueReceiveFromISR(m_can_tx_qs[cidx], &msg, NULL)
            ) {
                ok = CAN_tx_now(CANx, &msg);
            }
        }
        vPortExitCritical();
    }

    return ok;
}

bool CAN_rx (can_t can, can_msg_t *pCanMsg, uint32_t timeout_ms)
{
    bool ok = false;

    if (CAN_VALID(can) && pCanMsg)
    {
        if (taskSCHEDULER_RUNNING == xTaskGetSchedulerState()) {
            ok = xQueueReceive(m_can_rx_qs[CAN_INDEX(can)], pCanMsg, OS_MS(timeout_ms));
        }
        else {
            uint64_t msg_timeout = sys_get_uptime_ms() + timeout_ms;
            while (! (ok = xQueueReceiveFromISR(m_can_rx_qs[CAN_INDEX(can)], pCanMsg, NULL))) {
                if (sys_get_uptime_ms() > msg_timeout) {
                    break;
                }
            }
        }
    }

    return ok;
}

bool CAN_is_bus_off(can_t can)
{
    const uint32_t bus_off_mask = (1 << 7);
    const volatile LPC_CAN_TypeDef *pCanX = CAN_ENUM_TO_REGS(can);
    return (NULL == pCanX) ? true : !!(pCanX->GSR & bus_off_mask);
}

void CAN_reset_bus(can_t can)
{
    volatile LPC_CAN_TypeDef *pCanX = CAN_ENUM_TO_REGS(can);
    if (pCanX) {
        pCanX->MOD = can_mod_normal;
    }
}

can_std_id_t CAN_gen_sid(can_t can, uint16_t id)
{
    can_std_id_t ret = { id, 0, (0xffff == id) ? 1 : 0, can };
    return ret;
}

can_ext_id_t CAN_gen_eid(can_t can, uint32_t id)
{
    can_ext_id_t ret = { id, can };
    return ret;
}

bool CAN_fullcan_add_entry(can_t can, can_std_id_t id1, can_std_id_t id2)
{
    /* Return if invalid CAN given */
    if (!CAN_VALID(can)) {
        return false;
    }

    /* Check for enough room for more FullCAN entries
     * Each entry takes 2-byte entry, and 12-byte message RAM.
     */
    const uint16_t existing_entries = CAN_fullcan_get_num_entries();
    const uint16_t size_per_entry = sizeof(can_std_id_t) + sizeof(can_fullcan_msg_t);
    if ((existing_entries * size_per_entry) >= sizeof(LPC_CANAF_RAM->mask)) {
        return false;
    }

    /* Locate where we should write the next entry */
    uint8_t *base = (uint8_t*) &(LPC_CANAF_RAM->mask[0]);
    uint8_t *next_entry_ptr = base + LPC_CANAF->SFF_sa;

    /* Copy the new entry into the RAM filter */
    LPC_CANAF->AFMR = afmr_disabled;
    do {
        can_std_id_t entries[] = { id1, id2 };
        memcpy(next_entry_ptr, &entries, sizeof(entries));

        /* The new start of Standard Frame Filter is after the two new entries */
        const uint32_t new_sff_sa = LPC_CANAF->SFF_sa + sizeof(id1) + sizeof(id2);
        LPC_CANAF->SFF_sa = new_sff_sa;

        /* Next filters start at SFF_sa (they are disabled) */
        LPC_CANAF->SFF_GRP_sa = new_sff_sa;
        LPC_CANAF->EFF_sa     = new_sff_sa;
        LPC_CANAF->EFF_GRP_sa = new_sff_sa;
        LPC_CANAF->ENDofTable = new_sff_sa;
    } while(0);
    LPC_CANAF->AFMR = afmr_fullcan;

    return true;
}

can_fullcan_msg_t* CAN_fullcan_get_entry_ptr(can_std_id_t fc_id)
{
    /* Number of entries depends on how far SFF_sa is from base of 0 */
    const uint16_t num_entries = CAN_fullcan_get_num_entries();
    uint16_t idx = 0;

    /* The FullCAN entries are at the base of the CAN RAM */
    const can_std_id_t *id_list = (can_std_id_t*) &(LPC_CANAF_RAM->mask[0]);

    /* Find the standard ID entered into the RAM
     * Once we find the ID, its message's RAM location is after
     * LPC_CANAF->ENDofTable based on the index location
     */
    for (idx = 0; idx < num_entries; idx++) {
        if (id_list[idx].id == fc_id.id) {
            break;
        }
    }

    can_fullcan_msg_t *real_entry = NULL;
    if (idx < num_entries) {
        /* If we find an index, we have to convert it to the actual message pointer */
        can_fullcan_msg_t *base_msg_entry = (can_fullcan_msg_t*)
                                                   (((uint8_t*) &(LPC_CANAF_RAM->mask[0])) + LPC_CANAF->ENDofTable);
        real_entry = (base_msg_entry + idx);
    }

    return real_entry;
}

bool CAN_fullcan_read_msg_copy(volatile can_fullcan_msg_t *pMsg, can_fullcan_msg_t *pMsgCopy)
{
    const uint8_t *can_ram_base = (uint8_t*) &(LPC_CANAF_RAM->mask[0]);
    const uint8_t *start = can_ram_base + LPC_CANAF->ENDofTable;        // Actual FullCAN msgs are stored after this
    const uint8_t *end   = can_ram_base + sizeof(LPC_CANAF_RAM->mask);  // Last byte of CAN RAM + 1

    /* Validate the input pointers.  pMsg must be within range of our RAM filter
     * where the actual FullCAN message should be stored at
     */
    const uint8_t *ptr = (uint8_t*) pMsg;
    if (ptr < start || ptr >= end || !pMsgCopy) {
        return false;
    }

    /* After HW writes new data, semaphore bits are changed to 0b11 */
    bool new_msg_received = (pMsg->semphr != 0);

    /* Clear the semaphore bits and read the message copy */
    pMsg->semphr = 0;
    *pMsgCopy = *pMsg;

    /* If semaphore bits change, then HW has updated the message so read it again */
    if (0 != pMsg->semphr) {
        new_msg_received  = true;
        *pMsgCopy = *pMsg;
    }

    return new_msg_received;
}

uint8_t CAN_fullcan_get_num_entries(void)
{
    return LPC_CANAF->SFF_sa / sizeof(can_std_id_t);
}

bool CAN_setup_filter(const can_std_id_t *std_id_list,           uint16_t sid_cnt,
                      const can_std_grp_id_t *std_group_id_list, uint16_t sgp_cnt,
                      const can_ext_id_t *ext_id_list,           uint16_t eid_cnt,
                      const can_ext_grp_id_t *ext_group_id_list, uint16_t egp_cnt)
{
    bool ok = true;

    // Count of standard IDs must be even
    if (sid_cnt & 1) {
        return false;
    }

    LPC_CANAF->AFMR = afmr_disabled;
    {
        /* Filter RAM is after the FulLCAN entries */
        uint8_t *can_ram_base = (uint8_t*)&(LPC_CANAF_RAM->mask[0]);
        uint8_t *filter_ram_base = can_ram_base + LPC_CANAF->SFF_sa; // Our filter RAM is after FullCAN entries

        /* FullCAN entries take up 2 bytes each at beginning RAM, and 12-byte sections at the end */
        uint8_t *end  = can_ram_base + sizeof(LPC_CANAF_RAM->mask) -
                        ( sizeof(can_fullcan_msg_t) * CAN_fullcan_get_num_entries());
        uint8_t *ptr = filter_ram_base;

        #define CAN_add_filter_list(list, ptr, end, cnt, entry_size)  \
                do { if (NULL != list) {                              \
                     if (ptr + (cnt * entry_size) < end) {            \
                         memcpy(ptr, list, (cnt * entry_size));       \
                         ptr += (cnt * entry_size);                   \
                } else { ok = false; } } } while(0)

        /* The sa (start addresses) are byte address offset from CAN RAM
         * and must be 16-bit (WORD) aligned
         * LPC_CANAF->SFF_sa should already be setup by FullCAN if used, or
         * set to zero by the can init function.
         */
        CAN_add_filter_list(std_id_list, ptr, end, sid_cnt, sizeof(can_std_id_t));

        LPC_CANAF->SFF_GRP_sa = (ptr - can_ram_base);
        CAN_add_filter_list(std_group_id_list, ptr, end, sgp_cnt, sizeof(can_std_grp_id_t));

        LPC_CANAF->EFF_sa = (ptr - can_ram_base);
        CAN_add_filter_list(ext_id_list, ptr, end, eid_cnt, sizeof(can_ext_id_t));

        LPC_CANAF->EFF_GRP_sa = (ptr - can_ram_base);
        CAN_add_filter_list(ext_group_id_list, ptr, end, egp_cnt, sizeof(can_ext_grp_id_t));

        /* End of table is where the FullCAN messages are stored */
        LPC_CANAF->ENDofTable = (ptr - can_ram_base);
    }

    /* If there was no FullCAN entry, then SFF_sa will be zero.
     * If it was zero, we just enable the AFMR, but if it was not zero, that means
     * FullCAN entry was added, so we restore AMFR to fullcan enable
     */
    LPC_CANAF->AFMR = (0 == LPC_CANAF->SFF_sa) ? afmr_enabled : afmr_fullcan;

    return ok;
}

#if CAN_TESTING
#include <stdio.h>
#define CAN_ASSERT(x)   if (!(x)) { printf("BUS: %s MOD: 0x%08x, GSR: 0x%08x\n"\
                                           "IER/ICR: 0x%08X/0x%08x BTR: 0x%08x"\
                                           "\nLine %i: %s\n", \
                                           CAN_is_bus_off(can1) ? "OFF" : "ON", \
                                           (int)LPC_CAN1->MOD, (int)LPC_CAN1->GSR, \
                                           (int)LPC_CAN1->IER, (int)LPC_CAN1->ICR, \
                                           (int)LPC_CAN1->BTR, \
                                           __LINE__, #x); return false; }
void CAN_test_bufoff_cb(void)
{
    puts("CB: BUS OFF");
}
void CAN_test_bufovr_cb(void)
{
    puts("CB: DATA OVR");
}

bool CAN_test(void)
{
    #define CAN_TEST_RX 0

    CAN_ASSERT(!CAN_init(can_inv, 100, 0, 0, NULL, NULL));
    CAN_ASSERT(CAN_init(can1, 100, 0, 0, CAN_test_bufoff_cb, CAN_test_bufovr_cb));
    CAN_ASSERT(LPC_CAN1->MOD == can_mod_reset);
    CAN_ASSERT(m_can_rx_qs[0] != NULL);
    CAN_ASSERT(m_can_tx_qs[0] != NULL);
    CAN_ASSERT(LPC_CANAF->SFF_sa     == 0);
    CAN_ASSERT(LPC_CANAF->SFF_GRP_sa == 0);
    CAN_ASSERT(LPC_CANAF->EFF_sa     == 0);
    CAN_ASSERT(LPC_CANAF->EFF_GRP_sa == 0);
    CAN_ASSERT(LPC_CANAF->ENDofTable == 0);

    CAN_reset_bus(can1);
    CAN_ASSERT(LPC_CAN1->MOD == can_mod_normal);

    /* Send 11-bit CANID message */
    uint32_t id = 0x100;
    can_msg_t msg;
    msg.frame = 0;
    msg.msg_id = id;
    msg.frame_fields.is_29bit = 0;
    msg.frame_fields.data_len = 7;
    msg.data.qword = 0x1122334455667788;
    CAN_ASSERT(!CAN_tx(can_inv, &msg, 0));
    CAN_ASSERT(!CAN_rx(can1, &msg, 0));

    /* Send msg and test receive */
    CAN_ASSERT(CAN_tx(can1, &msg, 0));

#if CAN_TEST_RX
    memset(&msg, 0, sizeof(msg));
    CAN_ASSERT(CAN_rx(can1, &msg, 1000));
    CAN_ASSERT(msg.msg_id = id);
    CAN_ASSERT(!msg.frame_fields.is_29bit);
#endif

    /* Test filters */
    CAN_ASSERT(0 == CAN_fullcan_get_num_entries());
    CAN_ASSERT(CAN_fullcan_add_entry(can1, CAN_gen_sid(can1, id), CAN_gen_sid(can1, id+1)));
    CAN_ASSERT(2 == CAN_fullcan_get_num_entries());
    CAN_ASSERT(4 == LPC_CANAF->SFF_sa);

    can_fullcan_msg_t *fc1 = CAN_fullcan_get_entry_ptr(CAN_gen_sid(can1, id));
    can_fullcan_msg_t *fc2 = CAN_fullcan_get_entry_ptr(CAN_gen_sid(can1, id+1));
    CAN_ASSERT(0 != fc1);
    CAN_ASSERT(0 != fc2);

#if CAN_TEST_RX
    /* Send message, see if fullcan captures it */
    msg.frame = 0;
    msg.msg_id = id;
    msg.frame_fields.is_29bit = 0;
    msg.frame_fields.data_len = 8;

    can_fullcan_msg_t fc_temp;
    CAN_ASSERT(!CAN_fullcan_read_msg_copy(fc1, &fc_temp));
    CAN_ASSERT(CAN_tx(can1, &msg, 0));
    CAN_ASSERT(CAN_fullcan_read_msg_copy(fc1, &fc_temp));
    CAN_ASSERT(fc1->msg_id == id);
#endif

    const can_std_id_t slist[]      = { CAN_gen_sid(can1, 0x100), CAN_gen_sid(can1, 0x110),   // 2 entries
                                        CAN_gen_sid(can1, 0x120), CAN_gen_sid(can1, 0x130)    // 2 entries
    };
    const can_std_grp_id_t sglist[] = { {CAN_gen_sid(can1, 0x150), CAN_gen_sid(can1, 0x200)}, // Group 1
                                        {CAN_gen_sid(can2, 0x300), CAN_gen_sid(can2, 0x400)}  // Group 2
    };
    const can_ext_id_t *elist       = NULL; // Not used, so set it to NULL
    const can_ext_grp_id_t eglist[] = { {CAN_gen_eid(can1, 0x3500), CAN_gen_eid(can1, 0x4500)} }; // Group 1

    /* Test filter setup */
    CAN_setup_filter(slist, 4, sglist, 2, elist, 0, eglist, 1);

    CAN_ASSERT(2 == CAN_fullcan_get_num_entries());
    CAN_ASSERT(LPC_CANAF->SFF_sa     == 4);
    CAN_ASSERT(LPC_CANAF->SFF_GRP_sa == 12);
    CAN_ASSERT(LPC_CANAF->EFF_sa     == 20);
    CAN_ASSERT(LPC_CANAF->EFF_GRP_sa == 20);
    CAN_ASSERT(LPC_CANAF->ENDofTable == 28);

    /* Send a message defined in filter */

    /* Send a message not defined in filter */

    return true;
}
#endif
