#ifndef _CONFIGC_FILE
#define _CONFIGC_FILE

#include "configc.h"

#define FILE_OPT_LAZY 1 << 0

struct file_t {
	const char *path;
	const char *desired_value;
	const char *owner;
	const char *group;
	mode_t mode;
	unsigned int flags;
};

int file_diff(struct file_t *file);
int file_apply(struct file_t *file);

#endif
