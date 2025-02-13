#ifndef _CONFIGC_H
#define _CONFIGC_H

#include "error.h"

/*
 * std libs
 */
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

/*
 * memory management wrappers
 */
void *xmalloc(size_t size);

/*
 * logging wrappers
 */
void error(const char *fmt, ...);
void die(const char *fmt, ...);
void BUG(const char *fmt, ...);

/*
 * a little macro that exposes a slightly more readeable way of writing
 * whether two strings are equals.
 */
#define str_matches(s1, s2) (strcmp(s1, s2) == 0)

/*
 * xread() and xwrite() will retry up to MAX_IO_RETRY
 */
#define MAX_IO_RETRY 1024

/*
 * define MAX_IO_SIZE just to have an upper boundary
 */
#define MAX_IO_SIZE 4096 * 4096

/*
 * IO functions
 */
int file_exists(const char *path);
int file_stat(const char *path, struct stat *sb);
int xopen(const char *path, unsigned int flags, mode_t mode);
ssize_t xread(int fd, void *buf, size_t len);
ssize_t xwrite(int fd, const void *buf, size_t len);
char *read_whole_file(const char *path, int *rc);

#endif
