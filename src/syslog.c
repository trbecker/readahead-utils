#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "config.h"

#define MSG_SIZE 1024 * sizeof(char)

void log_open()
{
	openlog(PACKAGE_NAME, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
	setlogmask(LOG_UPTO(LOG_DEBUG));
}

void log_close()
{
	closelog();
}

static void vlog(int level, const char *fmt, va_list *args)
{
	char *msg = malloc(MSG_SIZE);
	if (!msg)
		return;

	vsnprintf(msg, MSG_SIZE, fmt, *args);
	syslog(level, "%s", msg);

	free(msg);
}

#define GENERATE_LOGGER(name, level)		\
	void name(const char *fmt, ...) {	\
	va_list args;				\
	va_start(args, fmt);			\
	vlog(LOG_##level, fmt, &args);		\
	va_end(args);				\
}

GENERATE_LOGGER(debug, DEBUG);
GENERATE_LOGGER(info, INFO);
GENERATE_LOGGER(notice, NOTICE);
GENERATE_LOGGER(warn, WARNING);
GENERATE_LOGGER(err, ERR);
GENERATE_LOGGER(crit, CRIT);
GENERATE_LOGGER(alert, ALERT);
GENERATE_LOGGER(emerg, EMERG);
