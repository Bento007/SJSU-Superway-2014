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
 * @file logger.hpp
 * @brief Provides Logger base classes
 * @ingroup Utilities
 *
 * Version: 09232012    Made flush() protected
 * Version: 06192012    Initial
 */
#ifndef LOGGER_HPP_
#define LOGGER_HPP_



/**
 * This is the Log Buffer Base class that provides a way of logging a single message and acts like
 * a circular buffer because when the logger memory is full, a call to handleCompletedBuffer() is
 * made so that that Parent class can handle the data that is going to be over-written.
 *
 * @ingroup Utilities
 */
class LoggerBase
{
    public:
        /**
         * @{ Logger functions to log a warning or an error message
         * @param pMessage  The string or null terminated data to log
         * @param pFuncName Optional parameter: The function name calling this method
         * @param pFilename Optional parameter: The file name calling this method
         * @param lineNum   Optional parameter: The software code line number calling this method
         */
        void logError(const char* pMessage, const char* pFilename=0, const char* pFuncName=0, int lineNum=0);
        void logWarn (const char* pMessage, const char* pFilename=0, const char* pFuncName=0, int lineNum=0);
        void logInfo (const char* pMessage, const char* pFilename=0, const char* pFuncName=0, int lineNum=0);

        void logErrorPrintf(const char* fname, const char* funcName, int lineNum, const char* msg, ...);
        void logWarnPrintf (const char* fname, const char* funcName, int lineNum, const char* msg, ...);
        void logInfoPrintf (const char* fname, const char* funcName, int lineNum, const char* msg, ...);
        /** @} */

    protected:
        /**
         * Constructor
         * @param pBuff The buffer pointer to store the data until flush() is called
         * @param size  The size of the buffer pointer: pBuff
         */
        LoggerBase(char* pBuff, int size) :
            mpBuffPtr(pBuff), mBuffPtrSize(size), mBuffUsage(0)
        {
            if (0 == mpBuffPtr) {
                mpBuffPtr = new char[size];
            }
        }

        /// Virtual destructor of this abstract class
        virtual ~LoggerBase(){}

        /**
         * Logs a message in the buffer.
         * @note This is protected because the parent class should put semaphore around
         *          this method since this is not a thread-safe method.
         * @post If/While Buffer gets full, flush() is called
         */
        void log(const char* pMessage);

        /// Flushes the buffer using handleCompletedBuffer() and zeroes out mBuffInfo to prepare for re-use
        void flush();

        /**
         * The object inheriting this class needs to provide its way of flushing
         * the buffer when buffer becomes full or when flush() is called
         */
        virtual void handleCompletedBuffer(char* pBuff, unsigned int size)=0;

        /** @{ Virtual function that parent class can override */
        virtual void semTake(void){ }
        virtual void semGive(void){ }
        virtual unsigned int getTimestamp(void) { return 0; }
        /** @} */

    private:
        /**
         * Logs a message separated by commas that has 6 columns:
         *  Timestamp, pInfoType, Message, File name, Function Name, Line Number
         * @param pInfoType  The type of information being logged, such as "ERROR" or "INFO"
         * @param pMessage   The message to log, can be NULL pointer if no message
         * @param pFuncName  Optional parameter: The function name calling this method
         * @param pFilename  Optional parameter: The file name calling this method
         * @param lineNum    Optional parameter: The software code line number calling this method
         */
        void logCsvMessage(const char* pInfoType, const char* pMessage,
                           const char* pFilename=0, const char* pFuncName=0, int lineNum=0);

        char* mpBuffPtr;                 ///< Memory pointer to store buffer until flush()
        const unsigned int mBuffPtrSize; ///< Size allocated for mpBuffPtr
        unsigned int mBuffUsage;         ///< Actual usage of the mBuffInfo.pBufferPtr
};


#endif /* LOGGER_HPP_ */
