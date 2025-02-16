#include "lib/testlib.h"

#include <configc.h>
#include <sysctl.h>

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

static void ensure_sysctl_diff_works()
{
	struct sysctl_t same = {
		.key = "sysctl.basic",
		.desired_value = "configc",
	};
	assert(sysctl_diff(&same) == 0);

	struct sysctl_t different = {
		.key = "sysctl.basic",
		.desired_value = "unconfig",
	};
	assert(sysctl_diff(&different) == 1);
}

static void ensure_sysctl_apply_works()
{
	struct sysctl_t simple_config = {
		.key = "sysctl.basic",
		.desired_value = "newconfig",
	};
	assert(sysctl_diff(&simple_config) == 1);
	assert(sysctl_apply(&simple_config) == 0);
	assert(sysctl_diff(&simple_config) == 0);
}

int main()
{
	setup();
	Run("Ensure sysctl diff works", ensure_sysctl_diff_works);
	Run("Ensure sysctl apply works", ensure_sysctl_apply_works);
	clean();
}
