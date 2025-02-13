#ifndef _CONFIGC_SYSCTL
#define _CONFIGC_SYSCTL

#include "configc.h"

#ifndef SYSPROC_ROOT
#  define SYSPROC_ROOT "/proc/sys/"
#endif

struct sysctl_t {

	/*
	 * The sysctl key represents the path of the linux option inside the /proc/sys directory,
	 * following the `sysctl` command pattern of each parent dir separated by '.' chars.
	 *
	 *    net.ipv4.ip_forwarding
	 *    net.fs.nr_open
	 */
	const char *key;
	const char *desired;

	/* private values */
	char *path;
        char *act_value;
};

/*
 * Initialize sysctl configuration with a specified state
 */
struct sysctl_t *sysctl_init(const char *key, const char *value);

/*
 * Probe the system to get the value of sysstat configuration
 */
int sysctl_get(struct sysctl_t *sysctl);

/*
 * Shows the diff between actual vs desired state
 */
int sysctl_diff(struct sysctl_t *sysctl);

/*
 * Attempt to apply the configuration
 */
int sysctl_apply(struct sysctl_t *sysctl);

/*
 * Free memory
 */
void sysctl_free(struct sysctl_t *sysctl);

#endif
