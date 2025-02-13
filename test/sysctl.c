#include "lib/testlib.h"

#include <configc.h>
#include <sysctl.h>

static void ensure_sysctl_get_works()
{
	struct sysctl_t *sysctl = sysctl_init("sysctl.basic", "configc");

	assert(sysctl_get(sysctl) == 0);
	assert(str_matches(sysctl->act_value, "configc"));
}

static void ensure_sysctl_diff_works()
{
	struct sysctl_t *sysctl = sysctl_init("sysctl.basic", "unconfigc");

	assert(sysctl_get(sysctl) == 0);
	assert(str_matches(sysctl->desired, "unconfigc"));
	assert(str_matches(sysctl->act_value, "configc"));
	assert(sysctl_diff(sysctl) != 0);
}

static void ensure_sysctl_apply_works()
{
	struct sysctl_t *sysctl = sysctl_init("sysctl.basic", "unconfigc");

	assert(sysctl_get(sysctl) == 0);
	assert(sysctl_diff(sysctl) != 0);
	assert(sysctl_apply(sysctl) == 0);
	assert(str_matches(sysctl->act_value, "unconfigc"));
}

void setup()
{
	remkdir("/tmp/runfc", 0755);
	remkdir("/tmp/runfc/sysctl", 0755);
	create_file("/tmp/runfc/sysctl/basic", "configc", 0644);
}

void clean()
{
	unlink("/tmp/runfc/sysctl/basic");
	rmdir("/tmp/runfc/sysctl");
	rmdir("/tmp/runfc");
}

int main()
{
	setup();
	Run("Ensure sysctl get works", ensure_sysctl_get_works);
        Run("Ensure sysctl diff works", ensure_sysctl_diff_works);
	Run("Ensure sysctl apply works", ensure_sysctl_apply_works);
	clean();
}
