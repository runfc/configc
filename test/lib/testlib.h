#ifndef __CONFIGC_TESTLIB_H
#define __CONFIGC_TESTLIB_H

#include <assert.h>
#include <execinfo.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <configc.h>

/*
 * Wrapper for running the tests
 */
void Run(const char *desc, void (*fn)());

/*
 * str_matches is used to check when two strings should match exactly, char by char.
 * special chars are not stripped from the comparasion
 */
#define str_matches(s1, s2) (strcmp(s1, s2) == 0)

/*
 * remkdir() or `re-mkdir`, ensure that a given directory is always
 * clean by removing any current content.
 */
void remkdir(const char *path, mode_t mode);

/*
 * create_file() ensures the file path and content are written, and
 * the only content of the file by removing any pre-existent file
 */
void create_file(const char *path, const char *content, mode_t mode);

#endif
