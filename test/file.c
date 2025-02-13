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
	unlink("/tmp/runfc/file/basic");
	rmdir("/tmp/runfc/file");
	rmdir("/tmp/runfc");
}

static void ensure_file_get_works()
{
	struct file_t *file = file_init("/tmp/runfc/file/basic", "configcc", 0644, 0);

	assert(file_get(file) == 0);
	assert(file->act_value != NULL);
	assert(str_matches(file->act_value, "configc"));
}

static void ensure_file_get_lazily_works()
{
	struct file_t *file = file_init("/tmp/runfc/file/basic", "configcc", 0644, FILE_OPT_LAZY);

	assert(file_get(file) == 0);
	assert(file->act_value == NULL);
	assert(file_diff(file) != 0);
	assert(str_matches(file->act_value, "configc"));
}

static void ensure_diff_chmod_and_apply_works()
{
	struct file_t *file = file_init("/tmp/runfc/file/basic.chmod", "configcc", 0644, 0);

	create_file("/tmp/runfc/file/basic.chmod", "configcc", 0600);

	assert(file_get(file) == 0);
	assert(file_diff(file) == 1);
	assert(file_apply(file) == 0);
	assert(file_diff(file) == 0);

	struct stat sb;
	stat(file->path, &sb);
	assert((sb.st_mode & 0644) == 0644);
}


static void ensure_file_apply_works()
{
	struct stat sb;
	mode_t expected_mode = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IROTH;
	struct file_t *file = file_init("/tmp/runfc/file/basic", "configcc", 0644, 0);

	assert(file_get(file) == 0);
	assert(file->act_value != NULL);
	assert(str_matches(file->act_value, "configc"));
	assert(file_apply(file) == 0);
	assert(str_matches(file->act_value, "configcc"));

	assert(file_stat(file->path, &sb) != 0);
	assert(sb.st_mode & expected_mode);
}

int main()
{
	setup();
	Run("Ensure file get works", ensure_file_get_works);
	Run("ensure file get lazily works", ensure_file_get_lazily_works);
	Run("ensure file diff/apply works for chmod", ensure_diff_chmod_and_apply_works);
	Run("ensure file apply works", ensure_file_apply_works);
	clean();
}
