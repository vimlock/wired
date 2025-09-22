#include "../include/wired/wLog.h"
#include "../include/wired/wTerm.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

static FILE *wLogFile = NULL;
static int wLogLevel = W_LOG_DEBUG;
static bool wLogUseColor = true;

void wLogOpenStderr()
{
	wLogFile = stderr;
}

void wLogSetLevel(int level)
{
	wLogLevel = level;
}

const char * wLogLevelStr(int level)
{
	switch (level) {
		case W_LOG_DEBUG: return "DEBUG";
		case W_LOG_INFO:  return "INFO";
		case W_LOG_WARN:  return "WARN";
		case W_LOG_ERROR: return "ERROR";
		case W_LOG_FATAL: return "FATAL";
	}

	return "<invalid>";
}

const char *wLogLevelColor(int level)
{
	switch (level) {
		case W_LOG_DEBUG: return W_TERM_CYAN;
		case W_LOG_INFO:  return W_TERM_WHITE;
		case W_LOG_WARN:  return W_TERM_YELLOW;
		case W_LOG_ERROR: return W_TERM_RED;
		case W_LOG_FATAL: return W_TERM_RED;
	}

	return "";
}

static bool wLogIgnore(int level)
{
	return level < wLogLevel;
}

static const char *basename(const char *path)
{
	#if __linux__ || __APPLE__
		const char sep = '/';
	#else
		char sep = '\\';
	#endif

	const char *lastslash = strrchr(path, sep);
	if (lastslash)
		return lastslash + 1;
	else
		return path;
}

void wLog(int level, const char *file, int line, const char *fmt, ...)
{
	struct tm tm;
	time_t timer;

	if (!wLogFile)
		return;

	if (wLogIgnore(level))
		return;

	file = basename(file);

	timer = time(NULL);

#ifdef _WIN32
	localtime_s(&tm, &timer);
#else
	localtime_s(&timer, &tm);
#endif

	char timestr[32];
	strftime(timestr, sizeof(timestr), "%H:%M:%S", &tm);

	fprintf(wLogFile, "%s ", timestr);

	fprintf(wLogFile, "%s%5s%s ",
		wLogUseColor ? wLogLevelColor(level) : "",
		wLogLevelStr(level),
		wLogUseColor ? W_TERM_RESET : ""
	);

	char linestr[32];
	snprintf(linestr, sizeof(linestr), "%d:", line);

	fprintf(wLogFile, "%18s:%-5s ", file, linestr);

	fprintf(wLogFile, "%s", wLogUseColor?W_TERM_BOLD:"");
	va_list va;
	va_start(va, fmt);
	vfprintf(wLogFile, fmt, va);
	va_end(va);

	fprintf(wLogFile, "%s\n", wLogUseColor?W_TERM_RESET:"");
}
