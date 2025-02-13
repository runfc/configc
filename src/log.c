#ifndef __CONFIGC_LOG_H
#define __CONFIGC_LOG_H

#include "configc.h"

void error(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	puts("error: ");
	vfprintf(stderr, fmt, ap);
	va_end(ap);
}

void die(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	puts("error: ");
	vfprintf(stderr, fmt, ap);
	exit(128);
	va_end(ap);
}

void BUG(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	puts("BUG: ");
	vfprintf(stderr, fmt, ap);
	exit(127);
	va_end(ap);
}

#endif
