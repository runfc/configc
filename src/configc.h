#ifndef _CONFIGC_H
#define _CONFIGC_H

/*
 * std libs
 */
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

/*
 * logging wrappers
 */
void error(const char *fmt, ...);
void die(const char *fmt, ...);
void BUG(const char *fmt, ...);

/*
 * IO functions
 */
int file_exists(const char *path);

#endif
