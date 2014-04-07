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

#include <stdio.h>              // printf()
#include <string.h>
#include <time.h>

#include "FreeRTOS.h"
#include "task.h"               // uxTaskGetSystemState()

#include "command_handler.hpp"  // CMD_HANDLER_FUNC()
#include "rtc.h"                // Set and Get System Time
#include "sys_config.h"         // TERMINAL_END_CHARS
#include "lpc_sys.h"

#include "utilities.h"          // printMemoryInfo()
#include "storage.hpp"          // Get Storage Device instances
#include "fat/disk/spi_flash.h"
#include "spi_sem.h"
#include "file_logger.hpp"      // Logger class

#include "uart0.hpp"
#include "wireless.h"
#include "nrf_stream.hpp"

#include "io.hpp"
#include "shared_handles.h"
#include "scheduler_task.hpp"

#include "c_tlm_stream.h"
#include "c_tlm_var.h"



CMD_HANDLER_FUNC(taskListHandler)
{
    const int delayInMs = (int)cmdParams;  // cast parameter str to integer

    if(delayInMs > 0) {
        vTaskResetRunTimeStats();
        vTaskDelayMs(delayInMs);
    }

    // Enum to char : eRunning, eReady, eBlocked, eSuspended, eDeleted
    const char * const taskStatusTbl[] = { "RUN", "RDY", "BLK", "SUS", "DEL" };

    // Limit the tasks to avoid heap allocation.
    const unsigned portBASE_TYPE maxTasks = 16;
    xTaskStatusType status[maxTasks];
    unsigned long totalRunTime = 0;
    const unsigned portBASE_TYPE uxArraySize =
            uxTaskGetSystemState(&status[0], maxTasks, &totalRunTime);

    // For percentage calculations.
    totalRunTime /= 100UL;

    output.printf("%10s Sta Pr Stack CPU%%          Time\n", "Name", TIMER0_US_PER_TICK);
    for(unsigned portBASE_TYPE i = 0; i < uxArraySize; i++)
    {
        xTaskStatusType *e = &status[i];
        const unsigned int cpuPercent = (0 == totalRunTime) ? 0 : e->ulRunTimeCounter / totalRunTime;
        const uint32_t timeMs = (uint64_t) e->ulRunTimeCounter * TIMER0_US_PER_TICK / 1000;

        output.printf("%10s %s %2u %5u %4u %10u ms\n",
                      e->pcTaskName, taskStatusTbl[e->eCurrentState], e->uxBasePriority,
                      e->usStackHighWaterMark, cpuPercent, timeMs);
    }

    if (uxTaskGetNumberOfTasks() > maxTasks) {
        output.printf("** WARNING: Only reported first %u tasks\n", maxTasks);
    }

    return true;
}

CMD_HANDLER_FUNC(memInfoHandler)
{
    char buffer[256] = { 0 };
    sys_get_mem_info_str(buffer);
    output.putline(buffer);
    return true;
}

CMD_HANDLER_FUNC(healthHandler)
{
    Uart0 &u0 = Uart0::getInstance();

    unsigned int total = 0;
    unsigned int available = 0;
    Storage::getFlashDrive().getDriveInfo(&total, &available);

    float floatTemp = TS.getFarenheit();
    int floatSig1 = (int) floatTemp;
    int floatDec1 = ((floatTemp - floatSig1) * 10);
    rtc_t bt = sys_get_boot_time();

    unsigned int highest_write_count = 0;
    unsigned int highest_write_count_page = 0;
    if (flash_supports_metadata())
    {
        spi1_lock();
        /* Determine the page that has been written the most */
        unsigned int pages = flash_get_page_count();

        for (unsigned int i = 0; i < pages; i++) {
            const unsigned int write_count = flash_get_page_write_count(i);
            if (write_count > highest_write_count) {
                highest_write_count = write_count;
                highest_write_count_page = i;
            }
        }
        spi1_unlock();

        const int max_writes = 100 * 1000;
        int life = 100 - (100 * highest_write_count / max_writes);
        output.printf("Flash: %u/%u Life: %i%% (page %u written %u times)\n",
                        available, total, life, highest_write_count_page, highest_write_count);
    }
    else {
        output.printf("Flash: %u/%u\n", available, total);
    }

    output.printf( "Temp : %u.%u\n"
                    "Light: %u\n"
                    "Time     : %s"
                    "Boot Time: %u/%u/%u,%02u:%02u:%02u\n"
                    "Uart0 Watermarks: %u/%u (rx/tx)\n",
                    floatSig1, floatDec1,
                    LS.getRawValue(),
                    rtc_get_date_time_str(),
                    bt.month, bt.day, bt.year, bt.hour, bt.min, bt.sec,
                    u0.getRxQueueWatermark(), u0.getTxQueueWatermark()
    );
    return true;
}

CMD_HANDLER_FUNC(timeHandler)
{
    rtc_t time;

    /**
     * If cmdParam contains "set" with six spaces, we can parse the time
     * Example: set 11 30 2014 8 25 0 1
     */
    if(cmdParams.beginsWith("set"))
    {
        unsigned m, d, y, hr, mn, sc, w;
        if( 7 != cmdParams.scanf("%*s %u %u %u %u %u %u %u", &m, &d, &y, &hr, &mn, &sc, &w))
        {
            return false;
        }

        time.month = m; time.day = d; time.year = y;
        time.hour = hr, time.min = mn; time.sec = sc;
        time.dow = w;
        rtc_settime(&time);
    }

    output.printf("%s", rtc_get_date_time_str());
    return true;
}

CMD_HANDLER_FUNC(logHandler)
{
    if (cmdParams == "flush") {
        FileLogger::getInstance().flush();
        output.putline("Log(s) have been flushed");
    }
    else {
        LOG_INFO(cmdParams());
        output.printf("Logged: |%s|\n", cmdParams());
    }
    return true;
}

CMD_HANDLER_FUNC(cpHandler)
{
    char *srcFile = NULL;
    char *dstFile = NULL;
    if(2 != cmdParams.tokenize(" ", 2, &srcFile, &dstFile)) {
        return false;
    }

    unsigned int readTimeMs = 0;
    unsigned int writeTimeMs = 0;
    unsigned int bytesTransferred = 0;
    FRESULT copyStatus = Storage::copy(srcFile, dstFile,
                                       &readTimeMs, &writeTimeMs, &bytesTransferred);

    if(FR_OK != copyStatus) {
        output.printf("Error %u copying |%s| -> |%s|\n", copyStatus, srcFile, dstFile);
    }
    else {
        output.printf("Finished!  Read: %u Kb/sec, Write: %u Kb/sec\n",
                      bytesTransferred/(0 == readTimeMs  ? 1 : readTimeMs),
                      bytesTransferred/(0 == writeTimeMs ? 1 : writeTimeMs));
    }
    return true;
}

CMD_HANDLER_FUNC(catHandler)
{
    // If -print was present, we will print to console
    const bool printToScreen = !cmdParams.erase("-noprint");
    cmdParams.trimStart(" ");
    cmdParams.trimEnd(" ");

    FIL file;
    if(FR_OK != f_open(&file, cmdParams(), FA_OPEN_EXISTING | FA_READ))
    {
        output.printf("Failed to open: %s\n", cmdParams());
    }
    else
    {
        // Extra char for null terminator
        char buffer[1024 + 1] = { 0 };
        unsigned int bytesRead = 0;
        unsigned int totalBytesRead = 0;

        const unsigned int startTime = sys_get_uptime_ms();
        while(FR_OK == f_read(&file, buffer, sizeof(buffer)-1, &bytesRead) && bytesRead > 0)
        {
            totalBytesRead += bytesRead;
            buffer[bytesRead] = '\0';

            if(printToScreen) {
                output.put(buffer);
            }
        }
        f_close(&file);

        if(!printToScreen) {
            const unsigned int timeTaken = sys_get_uptime_ms() - startTime;
            output.printf("\nRead %d bytes @ %d Kb/sec", totalBytesRead,
                          totalBytesRead/(0 == timeTaken  ? 1 : timeTaken));
        }
        output.putline("");
    }
    return true;
}

CMD_HANDLER_FUNC(lsHandler)
{
    DIR Dir;
    FILINFO Finfo;
    FATFS *fs;
    FRESULT returnCode = FR_OK;

    unsigned int fileBytesTotal = 0, numFiles = 0, numDirs = 0;
    #if _USE_LFN
        char Lfname[_MAX_LFN];
    #endif

    char *dirPath = NULL;
    char *lsOffsetStr = NULL;
    int lsOffset = 0;
    if (2 == cmdParams.tokenize(" ", 2, &dirPath, &lsOffsetStr)) {
        lsOffset = atoi(lsOffsetStr);
    }

    if (NULL == dirPath) {
        dirPath = (char*)"0:";
    }

    if (FR_OK != (returnCode = f_opendir(&Dir, dirPath))) {
        output.printf("Invalid directory: |%s| (Error %i)\n", dirPath, returnCode);
        return true;
    }

    // Offset the listing
    while(lsOffset-- > 0) {
        #if _USE_LFN
            Finfo.lfname = Lfname;
            Finfo.lfsize = sizeof(Lfname);
        #endif
        if (FR_OK != f_readdir(&Dir, &Finfo)) {
            break;
        }
    }

    output.printf("Directory listing of: %s\n\n", dirPath);
    for (;;)
    {
        #if _USE_LFN
            Finfo.lfname = Lfname;
            Finfo.lfsize = sizeof(Lfname);
        #endif

        char returnCode = f_readdir(&Dir, &Finfo);
        if ( (FR_OK != returnCode) || !Finfo.fname[0]) {
            break;
        }

        if (Finfo.fattrib & AM_DIR){
            numDirs++;
        }
        else{
            numFiles++;
            fileBytesTotal += Finfo.fsize;
        }
        output.printf("%c%c%c%c%c %u/%02u/%02u %02u:%02u %10lu %13s",
                (Finfo.fattrib & AM_DIR) ? 'D' : '-',
                (Finfo.fattrib & AM_RDO) ? 'R' : '-',
                (Finfo.fattrib & AM_HID) ? 'H' : '-',
                (Finfo.fattrib & AM_SYS) ? 'S' : '-',
                (Finfo.fattrib & AM_ARC) ? 'A' : '-',
                (Finfo.fdate >> 9) + 1980, (Finfo.fdate >> 5) & 15, Finfo.fdate & 31,
                (Finfo.ftime >> 11), (Finfo.ftime >> 5) & 63,
                Finfo.fsize, &(Finfo.fname[0]));

        // LFN names tend to increase memory requirements for output str, enable with caution
        #if (_USE_LFN)
        output.put(" - ");
        output.put(Lfname);
        #endif
        output.putline("");
    }
    output.printf("\n%4u File(s), %10d bytes total\n%4d Dir(s)", numFiles, fileBytesTotal, numDirs);

    if (f_getfree(dirPath, (DWORD*) &fileBytesTotal, &fs) == FR_OK)
    {
        output.printf(", %10dK bytes free\n", fileBytesTotal * fs->csize / 2);
    }
    return true;
}

CMD_HANDLER_FUNC(mkdirHandler)
{
    output.printf("Create directory '%s' : %s\n",
                   cmdParams(), (FR_OK == f_mkdir(cmdParams())) ? "OK" : "ERROR");
    return true;
}

CMD_HANDLER_FUNC(rmHandler)
{
    output.printf("Delete '%s' : %s\n",
                  cmdParams(), (FR_OK == f_unlink(cmdParams())) ? "OK" : "ERROR");
    return true;
}

CMD_HANDLER_FUNC(i2cIoHandler)
{
    bool read = cmdParams.beginsWithIgnoreCase("read");
    bool write = cmdParams.beginsWithIgnoreCase("write");
    bool discover = cmdParams.beginsWithIgnoreCase("discover");

    int addr = 0;
    int reg = 0;
    int data = 0;

    if (read) {
        if (2 != cmdParams.scanf("%*s %0x %0x", &addr, &reg)) {
            output.putline("Need device and register address");
            return false;
        }

        uint32_t data = I2C2::getInstance().readReg(addr, reg);
        output.printf("Read 0x%02X from 0x%02X::0x%02X\n", (data & 0xFF), addr, reg);
    }
    else if (write) {
        if (3 != cmdParams.scanf("%*s %0x %0x %0x", &addr, &reg, &data)) {
            output.putline("Need device, register address and data");
            return false;
        }

        I2C2::getInstance().writeReg(addr, reg, data);
        output.printf("Wrote 0x%02X to 0x%02X::0x%02X\n", data, addr, reg);
    }
    else if (discover) {
        for (int addr = 2; addr <= 254; addr += 2) {
            if (I2C2::getInstance().checkDeviceResponse(addr)) {
                output.printf("Found device with address %#4x\n", addr);
            }
        }
    }

    return (read || write || discover);
}

CMD_HANDLER_FUNC(mvHandler)
{
    char *srcFile = NULL;
    char *dstFile = NULL;
    if(2 != cmdParams.tokenize(" ", 2, &srcFile, &dstFile)) {
        return false;
    }
    else {
        output.printf("Move '%s' -> '%s' : %s\n",
                      srcFile, dstFile,
                      (FR_OK == f_rename(srcFile, dstFile))  ? "OK" : "ERROR");
    }
    return true;
}

CMD_HANDLER_FUNC(newFileHandler)
{
    const char end_file = '~';
    int timeout_ms = OS_MS(10 * 1000);

    FIL file;
    if (FR_OK != f_open(&file, cmdParams(), FA_WRITE | FA_CREATE_ALWAYS)) {
        output.printf("Unable to open '%s' to write the file\n", cmdParams());
        return true;
    }

    char c = 0;
    UINT bw = 0;
    output.printf("End the file by using %c character.  %i is the timeout\n", end_file, timeout_ms);
    output.printf("Sorry, no backspace support :(\n");

    while (output.getChar(&c, timeout_ms) && c != end_file) {
        if (FR_OK != f_write(&file, &c, 1, &bw) && 1 != bw) {
            output.printf("Error occurred while writing the file\n");
        }
        else {
            output.putChar(c);
        }
    }

    f_close(&file);
    return true;
}

CMD_HANDLER_FUNC(dcpHandler)
{
    DIR Dir;
    FILINFO Finfo;
    FRESULT returnCode = FR_OK;
    #if _USE_LFN
        char Lfname[_MAX_LFN];
    #endif

    char *srcDir = NULL;
    char *dstDir = NULL;
    if(2 != cmdParams.tokenize(" ", 2, &srcDir, &dstDir)) {
        output.putline("ERROR: Give me source and destination directory separated by a space");
        return true;
    }

    /* Check destination directory */
    if (FR_OK != (returnCode = f_opendir(&Dir, dstDir))) {
        output.printf("Invalid destination directory: |%s|\n", dstDir);
        return true;
    }
    /* Check source directory */
    if (FR_OK != (returnCode = f_opendir(&Dir, srcDir))) {
        output.printf("Invalid source directory: |%s|\n", srcDir);
        return true;
    }

    STR_ON_STACK(src, 256);
    STR_ON_STACK(dst, 256);
    for (;;)
    {
        #if _USE_LFN
            Finfo.lfname = Lfname;
            Finfo.lfsize = sizeof(Lfname);
        #endif

        /* If no more files */
        if ( (FR_OK != f_readdir(&Dir, &Finfo)) || !Finfo.fname[0]) {
            break;
        }

        /* If not a directory */
        if (!(Finfo.fattrib & AM_DIR))
        {
            src.printf("%s/%s", srcDir, Finfo.fname);
            dst.printf("%s/%s", dstDir, Finfo.fname);

            output.printf("Copy %s -> %s : %d Bytes : %s\n",
                           src(), dst(), Finfo.fsize,
                           (FR_OK == Storage::copy(src(), dst())) ? "OK" : "ERROR");
        }
    }
    return true;
}

CMD_HANDLER_FUNC(storageHandler)
{
    if(cmdParams == "format sd") {
        output.putline((FR_OK == Storage::getSDDrive().format()) ? "Format OK" : "Format ERROR");
    }
    else if(cmdParams == "format flash") {
        output.putline((FR_OK == Storage::getFlashDrive().format()) ? "Format OK" : "Format ERROR");
    }
    else if(cmdParams == "mount sd") {
        output.putline(FR_OK == Storage::getSDDrive().mount() ? "SD Card mounted" : "Error mounting SD Card");
    }
    else if(cmdParams == "mount flash") {
        output.putline(FR_OK == Storage::getFlashDrive().mount() ? "Flash mounted" : "Error mounting Flash Memory");
    }
    else {
        return false;
    }
    return true;
}

CMD_HANDLER_FUNC(rebootHandler)
{
    output.putline("Rebooting System");

    // Flush out everything before we reboot
    FileLogger::getInstance().flush();

    vTaskDelayMs(2000);
    sys_reboot();
    return true;
}

#if (ENABLE_TELEMETRY)
static void stream_tlm(const char *s, void *arg)
{
    CharDev *out = (CharDev*) arg;
    while (*s) {
        out->putChar(*s);
        s++;
    }
}

CMD_HANDLER_FUNC(telemetryHandler)
{
    if(cmdParams.getLen() == 0) {
        tlm_stream_all(stream_tlm, &output);
    }
    else if(cmdParams == "save") {
        FILE *fd = fopen(DISK_TLM_NAME, "w");
        tlm_stream_one_file(tlm_component_get_by_name(DISK_TLM_NAME), fd);
        fclose(fd);
        output.putline("Telemetry was saved to disk");
    }
    else if(cmdParams.beginsWithIgnoreCase("get")) {
        char *compName = NULL;
        char *varName = NULL;
        if (3 == cmdParams.tokenize(" ", 3, NULL, &compName, &varName)) {
            char buffer[256] = { 0 };
            if (tlm_variable_get_value(compName, varName, &buffer[0], sizeof(buffer))) {
                output.putline(buffer);
            }
            else {
                output.putline("Error locating or printing variable value");
            }
        }
        else {
            output.putline("Required parameters: 'get <comp name> <var name>");
        }
    }
    else {
        char *compName = NULL;
        char *varName = NULL;
        char *varVal = NULL;
        if (3 != cmdParams.tokenize(" ", 3, &compName, &varName, &varVal)) {
            output.putline("ERROR: See 'help telemetry'");
        }
        else {
            if (tlm_variable_set_value(compName, varName, varVal)) {
                output.printf("%s:%s set to %s\n", compName, varName, varVal);
            } else {
                output.printf("Failed to set %s:%s to %s\n", compName, varName, varVal);
            }
        }
    }
    return true;
}
#endif

CMD_HANDLER_FUNC(learnIrHandler)
{
    xSemaphoreHandle learn_sem = scheduler_task::getSharedObject(shared_learnSemaphore);

    if (learn_sem)
    {
        xSemaphoreGive(learn_sem);
        output.putline("Learning mode enabled");
    }
    else
    {
        output.putline("ERROR: Semaphore was NULL");
    }

    return true;
}
