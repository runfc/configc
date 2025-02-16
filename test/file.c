#include "lib/testlib.h"

#include <configc.h>
#include <file.h>

static void setup()
{
	remkdir("/tmp/runfc", 0755);
	remkdir("/tmp/runfc/file", 0755);
	create_file("/tmp/runfc/file/basic", "configc", 0644);
}

static void clean()
{
	unlink("/tmp/runfc/fil/apply");
	unlink("/tmp/runfc/file/basic");
	rmdir("/tmp/runfc/file");
	rmdir("/tmp/runfc");
}

static void ensure_file_diff_works()
{
	struct file_t no_diff = {
		.path = "/tmp/runfc/file/basic",
		.desired_value = "configc",
		.mode = 0644,
		.owner = "root",
		.group = "root",
		.flags = 0,
	};
	assert(file_diff(&no_diff) == 0);

	struct file_t with_different_content = {
		.path = "/tmp/runfc/file/basic",
		.desired_value = "configcc",
		.mode = 0644,
		.owner = "root",
		.group = "root",
		.flags = 0,
	};
	assert(file_diff(&with_different_content) == 1);

	struct file_t with_different_content_same_len = {
		.path = "/tmp/runfc/file/basic",
		.desired_value = "configd",
		.mode = 0644,
		.owner = "root",
		.group = "root",
		.flags = 0,
	};
	assert(file_diff(&with_different_content_same_len) == 1);
}

static void ensure_file_apply_works()
{
	struct file_t new_file = {
		.path = "/tmp/runfc/file/apply",
		.desired_value = "configc",
		.mode = 0644,
		.owner = "root",
		.group = "root",
		.flags = 0,
	};
	assert(file_apply(&new_file) == 0);
	/* ensure's file_diff() works after our apply */
	assert(file_diff(&new_file) == 0);
}

int main()
{
	setup();
	Run("Ensure file_diff() works", ensure_file_diff_works);
	Run("Ensure file_apply() works", ensure_file_apply_works);
	clean();
}
