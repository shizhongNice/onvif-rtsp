#ifndef _XCAM_LOG_H_
#define _XCAM_LOG_H_

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */


void _log(int level, const char* tag, const char* file, const char* func, int line, const char* fmt, ...);


enum {
	LOG_LEVEL_VERBOSE,
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_INFO,
	LOG_LEVEL_WARN,
	LOG_LEVEL_ERROR,
};


#define LOG(level, tag, fmt, ...)				\
    _log(level, tag, __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)


#define LOG_VER(tag, fmt, ...)	LOG(LOG_LEVEL_VERBOSE, tag, fmt, ##__VA_ARGS__)
#define LOG_DBG(tag, fmt, ...)	LOG(LOG_LEVEL_DEBUG, tag, fmt, ##__VA_ARGS__)
#define LOG_INF(tag, fmt, ...)	LOG(LOG_LEVEL_INFO, tag, fmt, ##__VA_ARGS__)
#define LOG_WAN(tag, fmt, ...)	LOG(LOG_LEVEL_WARN, tag, fmt, ##__VA_ARGS__)
#define LOG_ERR(tag, fmt, ...)	LOG(LOG_LEVEL_ERROR, tag, fmt, ##__VA_ARGS__)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif

