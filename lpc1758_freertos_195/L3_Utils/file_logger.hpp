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

/**
 * @file fileLogger.hpp
 * @brief This is a logger that logs data to a file on the system such as an SD Card.
 * @ingroup Utilities
 *
 * Version: 09232012    modified flush() to use semaphores
 * Version: 06192012    Initial
 */
#ifndef FILE_LOGGER_HPP__
#define FILE_LOGGER_HPP__


/// The size of the buffer, multiples of 512 recommended for File Logger
#define LOGGER_BUFFER_SIZE      1024


/// This macro will log INFO message without filename, function name, and line number
#define LOG_INFO_SIMPLE(pMSG)               FileLogger::getInstance().logInfo   (pMSG)
#define LOG_INFO_SIMPLE_PRINTF(msg, p...)   FileLogger::getInstance().logInfoPrintf(0, 0, 0, msg, ##p)

/**@{ Macros to log a message that picks up filename, function name, and line number that logged the message */
#define LOG_ERROR(pMSG)         FileLogger::getInstance().logError (pMSG, __FILE__, __FUNCTION__, __LINE__)
#define LOG_WARN(pMSG)          FileLogger::getInstance().logWarn  (pMSG, __FILE__, __FUNCTION__, __LINE__)
#define LOG_INFO(pMSG)          FileLogger::getInstance().logInfo  (pMSG, __FILE__, __FUNCTION__, __LINE__)
/** @} */

/** @{ Macros to log a message with printf option */
#define LOG_ERROR_PRINTF(msg, p...)  FileLogger::getInstance().logErrorPrintf(__FILE__, __FUNCTION__, __LINE__, msg, ##p)
#define LOG_WARN_PRINTF(msg, p...)   FileLogger::getInstance().logWarnPrintf(__FILE__, __FUNCTION__, __LINE__, msg, ##p)
#define LOG_INFO_PRINTF(msg, p...)   FileLogger::getInstance().logInfoPrintf(__FILE__, __FUNCTION__, __LINE__, msg, ##p)
/** @} */

#include "src/logger.hpp"
#include "singleton_template.hpp"
#include "FreeRTOS.h"
#include "semphr.h"



/**
 * File Logger Class
 * @ingroup Utilities
 *
 * This is an instance of logger that will print the logged data once
 * the LoggerBase becomes full. This class also provides semaphore
 * and getTimestamp() function to complete the abstract logger class.
 *
 * @warning DO NOT LOG IF YOU ARE INSIDE AN ISR because if Logger becomes full
 *          then we might try to use FATFS API which uses FreeRTOS API
 */
class FileLogger : public LoggerBase, public SingletonTemplate<FileLogger>
{
    public:
        void flush();
        // LoggerBase exposes remaining public API

    protected:
        /// Protected Constructor for Singleton class
        FileLogger();

        /** @{ Virtual function overrides */
        void handleCompletedBuffer(char* pBuff, unsigned int size);
        void semTake(void);
        void semGive(void);
        unsigned int getTimestamp(void);

    private:
        SemaphoreHandle_t mSemHandle;    ///< Semaphore for the LoggerBase

        /// Friend class used for Singleton Template
        friend class SingletonTemplate<FileLogger>;
};

/**
 * Simple file logger.  The purpose is to expose a single printf message to log.
 * This class is not multithreaded safe.
 */
class SimpleLogger : protected LoggerBase
{
    public:
        SimpleLogger(const char* filename, char *buff, int size);
        void printf(const char *msg, ...);
        void flush(void);

    protected:
        SimpleLogger();
        void handleCompletedBuffer(char* pBuff, unsigned int size);
        const char *mpFilename;
};

#endif /* FILE_LOGGER_HPP__ */
