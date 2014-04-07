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

#include "file_logger.hpp"
#include "storage.hpp"
#include "utilities.h"
#include "lpc_sys.h"



void FileLogger::flush()
{
    semTake();
    {
        LoggerBase::flush();
    }
    semGive();
}

FileLogger::FileLogger() :
        LoggerBase(NULL, LOGGER_BUFFER_SIZE)
{
    mSemHandle = xSemaphoreCreateMutex();
}

void FileLogger::handleCompletedBuffer(char* pBuff, unsigned int size)
{
    bool success = (FR_OK == Storage::append(LOGGER_FILE_NAME, pBuff, size));

    if (!success)
    {
        puts("Error logging data to file, here is a printout of the data: ");
        for (unsigned int i = 0; i < size; i++)
        {
            putchar(pBuff[i]);
        }
    }
}

void FileLogger::semTake(void)
{
    if (is_freertos_running())
        xSemaphoreTake(mSemHandle, portMAX_DELAY);
}
void FileLogger::semGive(void)
{
    if (is_freertos_running())
        xSemaphoreGive(mSemHandle);
}
unsigned int FileLogger::getTimestamp(void)
{
    return sys_get_uptime_ms();
}

SimpleLogger::SimpleLogger(const char *filename, char *buff, int size) :
        LoggerBase(buff, size),
        mpFilename(filename)
{
}

void SimpleLogger::printf(const char *msg, ...)
{
    va_list args;
    va_start(args, msg);
    char buff[256] = { 0 };

    vsnprintf(buff, sizeof(buff)-1, msg, args);

    LoggerBase::log(buff);
    va_end(args);
}

void SimpleLogger::flush(void)
{
    LoggerBase::flush();
}
void SimpleLogger::handleCompletedBuffer(char* pBuff, unsigned int size)
{
    bool success = (FR_OK == Storage::append(mpFilename, pBuff, size));
    (void) success; // Do something if failure ???
}

