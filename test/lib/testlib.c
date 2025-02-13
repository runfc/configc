#include "testlib.h"

const char *trace = NULL;

void Run(const char *desc, void (*fn)())
{
	printf("Running test: %s\n", desc);
	fn();
}

void remkdir(const char *path, mode_t mode)
{
	if (file_exists(path))
		unlink(path);
	mkdir(path, mode);
}

void create_file(const char *path, const char *content, mode_t mode)
{
	int fd = 0;

	if (file_exists(path))
		unlink(path);

	fd = xopen(path, O_CREAT | O_WRONLY, mode);
	if (fd < 0)
		BUG("TEST: Unable to open file: %s\n", path);

	if (write(fd, content, strlen(content)) < 0)
		BUG("TEST: Unable to write content to file '%s', with content: %s\n", path, content);
}
