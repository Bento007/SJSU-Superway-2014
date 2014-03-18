#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

#include "ff.h"
#include "storage.hpp"
#include "fat/disk/spi_flash.h"
#include "command_handler.hpp"
#include "lpc_sys.h"



/** @{ Special registers
 * These should be consistent with the bootloader such that it can see
 * our data, and program the flash memory from spi flash.
 */
#define SP_REG_IMG_SIG          0xDEADD00D      ///< SP_REG_IMG_PROG is set to this value to program flash
#define SP_REG_IMG_PROG         LPC_RTC->GPREG0 ///< Image program "command" is stored here
#define SP_REG_IMG_FILENAME     LPC_RTC->GPREG1 ///< Null terminated image name (up to 12 chars)
/** @} */


CMD_HANDLER_FUNC(flashProgHandler)
{
    FIL file;
    const int maxChars = 12;

    if (cmdParams.getLen() >= maxChars) {
        output.printf("Filename should be less than %i chars\n", maxChars);
    }
    if (FR_OK == f_open(&file, cmdParams(), FA_OPEN_EXISTING | FA_READ))
    {
        // Save the parameters that (hyperload) bootloader will look at
        SP_REG_IMG_PROG = SP_REG_IMG_SIG;

        /**
         * We can write up to 12 chars at special register
         * @warning : using strcpy() will not work
         */
        char *imgName = (char*) &(SP_REG_IMG_FILENAME);
        memcpy(imgName, cmdParams.c_str(), maxChars);

        output.printf("File (%u bytes) will be programmed.\n"
                      "Rebooting now to upgrade firmware!\n",
                      file.fsize);

        f_close(&file);
        vTaskDelay(1000);
        sys_reboot();
    }
    else {
        output.printf("Unable to open '%s'\n", cmdParams());
    }
    return true;
}

CMD_HANDLER_FUNC(getFileHandler)
{
    const int maxBufferSize = 1024;
    static char *spBuffer = (char*) malloc(maxBufferSize);

    /**
     * Packet format:
     * buffer <offset> <num bytes> ...
     * commit <filename> <file offset> <num bytes from buffer>
     */
    if (cmdParams.beginsWithIgnoreCase("commit"))
    {
        char filename[128] = { 0 };
        int offset = 0;
        int size = 0;
        cmdParams.scanf("%*s %128s %i %i", &filename[0], &offset, &size);

        FRESULT writeStatus = FR_INT_ERR;
        if(0 == offset) {
            writeStatus = Storage::write(filename, spBuffer, size);
        }
        else {
            writeStatus = Storage::append(filename, spBuffer, size, offset);
        }
        output.printf(FR_OK == writeStatus ? "OK\n" : "File write error\n");
    }
    else if (cmdParams.beginsWithIgnoreCase("buffer"))
    {
        int offset = 0;
        int numBytes = 0;
        int checksum = 0;
        char c = 0;

        cmdParams.scanf("%*s %i %i", &offset, &numBytes);

        if (offset + numBytes > maxBufferSize) {
            output.printf("ERROR: Max buffer size is %i bytes\n", maxBufferSize);
            return true;
        }

        for(int i=offset; i - offset < numBytes && i < maxBufferSize; i++) {
            if(! output.getChar(&c, OS_MS(2000))) {
                output.printf("ERROR: TIMEOUT\n");
                return true;
            }

            spBuffer[i] = c;
            checksum += c;
        }

        output.printf("Checksum %i\n", checksum);
    }
    else {
        return false;
    }

    return true;
}
