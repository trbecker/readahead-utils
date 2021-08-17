#ifndef __ra_log_h__
#define __ra_log_h__
#include <stdarg.h>

enum log_level {
	DEBUG, INFO
};

extern void log_open();
extern void log_close();

extern void debug(const char *, ...);
extern void info(const char *, ...);
extern void notice(const char *, ...);
extern void warn(const char *, ...);
extern void err(const char *, ...);
extern void crit(const char *, ...);
extern void alert(const char *, ...);
extern void emerg(const char *, ...);
#endif
