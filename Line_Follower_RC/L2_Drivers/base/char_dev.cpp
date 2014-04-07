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



#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"

#include "char_dev.hpp"
#include "utilities.h"      // system_get_timer_ms();



/**
 * @{ Common members across all instances of this class
 * We share a single instance of mpPrintfMem such that all classes inheriting
 * this object can share the memory to reduce the memory footprint.
 * This is protected by a semaphore if used in FreeRTOS tasks simultaneously
 * so sharing wouldn't pose any problems
 */
char* CharDev::mspPrintfMem = NULL;
uint16_t CharDev::msPrintfMemSize = 0;
xSemaphoreHandle CharDev::msPrintfSemaphore = NULL;
/** @} */


bool CharDev::put(const char* pString, unsigned int timeout)
{
    if (!pString) {
        return false;
    }

    while(*pString) {
        if(!putChar(*pString++, timeout)) {
            return false;
        }
    }

    return true;
}

void CharDev::putline(const char* pBuff, unsigned int timeout)
{
    this->put(pBuff, timeout);
    this->put("\r\n", timeout);
}

bool CharDev::gets(char* pBuff, int maxLen, unsigned int timeout)
{
    char c = 0;
    int charCount = 0;
    bool success = false;

    while(getChar(&c, timeout)) {
        if ('\r' != c && '\n' != c) {
            *pBuff++ = c;
        }
        if(++charCount >= maxLen) {
            break;
        }
        if('\n' == c) {
            success = true;
            break;
        }
    }

    // If we didn't get any characters, don't null terminate the string
    if(charCount > 0) {
        *pBuff = '\0';
    }

    return success;
}

int CharDev::printf(const char *format, ...)
{
    if (taskSCHEDULER_RUNNING == xTaskGetSchedulerState()) {
        xSemaphoreTake(msPrintfSemaphore, portMAX_DELAY);
    }

        int len = 0;
        va_list args;
        va_start(args, format);

        do {
            va_list args_copy;
            va_copy(args_copy, args);
            len = vsnprintf(mspPrintfMem, msPrintfMemSize, format, args_copy);
            va_end(args_copy);

            if (len >= msPrintfMemSize) {
                const int align = 16;
                msPrintfMemSize = (align + ((len/align) * align));
                /* TO DO :
                 * Do not know why realloc() doesn't work.  It is a combination of C++ class
                 * use combined with va_args and realloc itself.  It seems to work in vector
                 * and str classes though.
                 */
                if (1) {
                    free(mspPrintfMem);
                    mspPrintfMem = (char*) malloc(msPrintfMemSize);
                }
                else {
                    mspPrintfMem = (char*) realloc(mspPrintfMem, msPrintfMemSize);
                }
            }
            else {
                break;
            }
        } while (mspPrintfMem);

        va_end(args);
        this->put(mspPrintfMem);

    if (taskSCHEDULER_RUNNING == xTaskGetSchedulerState()) {
        xSemaphoreGive(msPrintfSemaphore);
    }

    return len;
}

int CharDev::scanf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int parsed = 0;
    char buff[256] = { 0 };

    if (this->gets(buff, sizeof(buff))) {
        parsed = vsscanf(buff, format, args);
    }
    va_end(args);
    return parsed;
}

CharDev::CharDev() : mReady(false)
{
    if (!msPrintfSemaphore) {
        msPrintfSemaphore = xSemaphoreCreateMutex();
    }
}

CharDev::~CharDev()
{
    /**
     * We do not release the memory here because this object shouldn't self-destruct
     * because other instances may be using it.  We can use a reference counter, but
     * we'll keep things simple and not perform this clean-up
     */
#if 0
    if (mspPrintfMem) {
        free(mspPrintfMem);
    }
#endif
}
