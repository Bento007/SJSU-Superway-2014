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

#include "logger.hpp"
#include "rtc.h"
#include <stdlib.h>   // itoa()
#include <stdio.h>    // sprintf()
#include <string.h>   // strlen()
#include <stdarg.h>


void LoggerBase::logError(const char* pMessage, const char* pFilename, const char* pFuncName, int lineNum)
{
    logCsvMessage("ERROR", pMessage, pFilename, pFuncName, lineNum);
}
void LoggerBase::logWarn (const char* pMessage, const char* pFilename, const char* pFuncName, int lineNum)
{
    logCsvMessage("WARN", pMessage, pFilename, pFuncName, lineNum);
}
void LoggerBase::logInfo(const char* pMessage, const char* pFilename, const char* pFuncName, int lineNum)
{
    logCsvMessage("INFO", pMessage, pFilename, pFuncName, lineNum);
}


void LoggerBase::logErrorPrintf  (const char* fname, const char* funcName, int lineNum, const char* msg, ...)
{
    va_list args;   va_start(args, msg);    char buff[256] = { 0 };
    vsnprintf(buff, sizeof(buff)-1, msg, args);

    logError(buff, fname, funcName, lineNum);
    va_end(args);
}
void LoggerBase::logWarnPrintf(const char* fname, const char* funcName, int lineNum, const char* msg, ...)
{
    va_list args;   va_start(args, msg);    char buff[256] = { 0 };
    vsnprintf(buff, sizeof(buff)-1, msg, args);

    logWarn(buff, fname, funcName, lineNum);
    va_end(args);
}
void LoggerBase::logInfoPrintf(const char* fname, const char* funcName, int lineNum, const char* msg, ...)
{
    va_list args;   va_start(args, msg);    char buff[256] = { 0 };
    vsnprintf(buff, sizeof(buff)-1, msg, args);

    logInfo(buff, fname, funcName, lineNum);
    va_end(args);
}


void LoggerBase::log(const char* pMessage)
{
    // while not enough room to store the message, keep flushing the buffer
    while(mBuffUsage+strlen(pMessage) > mBuffPtrSize)
    {
        const int lenOfMsg = strlen(pMessage);
        const int availableMem = mBuffPtrSize - mBuffUsage;
        const int lenToCopy = lenOfMsg < availableMem ? lenOfMsg : availableMem;

        memcpy(mpBuffPtr+mBuffUsage, pMessage, lenToCopy);
        mBuffUsage += lenToCopy;
        flush();

        pMessage += lenToCopy;
    }

    const int lenOfMsg = strlen(pMessage);
    memcpy(mpBuffPtr+mBuffUsage, pMessage, lenOfMsg);
    mBuffUsage += lenOfMsg;
}

void LoggerBase::flush()
{
    if(mBuffUsage > 0)
    {
        handleCompletedBuffer(mpBuffPtr, mBuffUsage);
        mBuffUsage = 0;
    }

    // Reset our buffer and zero it out.
    // memset(mpBuffPtr, 0, mBuffPtrSize);
}

void LoggerBase::logCsvMessage(const char* pInfoType, const char* pMessage,
                              const char* pFilename, const char* pFuncName, int lineNum)
{
    semTake();
    {
        const char comma[] = ",";
        const char parens[] = "()";
        const char newline[] = "\n";

        char buff[40] = { 0 };
        rtc_t time = rtc_gettime();
        sprintf(buff, "%u/%u,%02d:%02d:%02d,%u,%s,",
                (unsigned)time.month, (unsigned)time.day, (unsigned)time.hour,
                (unsigned)time.min, (unsigned)time.sec, getTimestamp(),
                pInfoType
                );
        log(buff);

        log(pMessage);
        log(comma);

        // Find the back-slash or forward-slash to get filename only, not absolute or relative path
        if(0 != pFilename) {
            char* pSlash = strrchr(pFilename, '/');
            // If forward-slash not found, find back-slash
            if(0 == pSlash) pSlash = strrchr(pFilename, '\\');
            if(0 != pSlash) pFilename = pSlash+1;

            log(pFilename);
        }
        log(comma);

        if(0 != pFuncName) {
            log(pFuncName);
            log(parens);
        }
        log(comma);

        if(0 != lineNum) {
            sprintf(buff, "%i", lineNum);
            log(buff);
        }
        log(newline);
    }
    semGive();
}
