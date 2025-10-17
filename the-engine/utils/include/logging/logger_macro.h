
#pragma once
#include "logger_utils.h"


#ifdef NDEBUG
    #define ENGINE_RELEASE 1
#endif

#ifdef ENGINE_RELEASE

    #define LOG_TRACE(logger, src, msg) ((void)0)
    #define LOG_DEBUG(logger, src, msg) ((void)0)
    
    #define LOG_INFO(logger, src, msg) (logger).info(src, msg)
    #define LOG_WARN(logger, src, msg) (logger).warn(src, msg)

    #define LOG_ERROR(logger, src, msg) (logger).error(src, msg)
    #define LOG_FATAL(logger, src, msg) (logger).fatal(src, msg)

#else

    #define LOG_TRACE(logger, src, msg) (logger).trace(src, msg)
    #define LOG_DEBUG(logger, src, msg) (logger).debug(src, msg)
    
    #define LOG_INFO(logger, src, msg) (logger).info(src, msg)
    #define LOG_WARN(logger, src, msg) (logger).warn(src, msg)

    #define LOG_ERROR(logger, src, msg) (logger).error(src, msg)
    #define LOG_FATAL(logger, src, msg) (logger).fatal(src, msg)

#endif
