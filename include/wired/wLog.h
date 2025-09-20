#pragma once

enum wLogLevel
{
	W_LOG_DEBUG,
	W_LOG_INFO,
	W_LOG_WARN,
	W_LOG_ERROR,
	W_LOG_FATAL,
	W_LOG_NONE,
};

void wLogOpenStderr();
void wLogSetLevel(int level);
const char * wLogLevelStr(int level);

#define wLogDebug(...) wLog(W_LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define wLogInfo(...)  wLog(W_LOG_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define wLogWarn(...)  wLog(W_LOG_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define wLogError(...) wLog(W_LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define wLogFatal(...) wLog(W_LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

void wLog(int level, const char *file, int line, const char *fmt, ...);
