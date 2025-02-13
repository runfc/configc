#ifndef _CONFIGC_FILE
#define _CONFIGC_FILE

#include "configc.h"

#define FILE_OPT_LAZY 1 << 0

struct file_t {
        char *path;
	char *desired;
	mode_t mode;

	/* private */
	unsigned int opts;
        char *act_value;
};

struct file_t *file_init(const char *path, const char *content, mode_t mode, unsigned int opts);
void file_free(struct file_t *path);

int file_get(struct file_t *file);
int file_diff(struct file_t *file);
int file_apply(struct file_t *file);

#endif
