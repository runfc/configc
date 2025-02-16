#ifndef _CONFIGC_SYSCTL
#define _CONFIGC_SYSCTL

#include "configc.h"

#ifndef SYSPROC_ROOT
#  define SYSPROC_ROOT "/proc/sys/"
#endif

struct sysctl_t {
	const char *key;
	const char *desired_value;
};

int sysctl_diff(struct sysctl_t *sysctl);
int sysctl_apply(struct sysctl_t *sysctl);

#endif
