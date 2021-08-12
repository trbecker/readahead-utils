#ifndef __ra_parser_util_h__
#define __ra_parser_util_h__
#include <stdarg.h>
#include <stdio.h>

static void debug(const char *fmt, ...)
{
#ifdef DEBUG
	va_list argp;
	va_start(argp, fmt);
	vfprintf(stderr, fmt, argp);
#endif
}
#endif
