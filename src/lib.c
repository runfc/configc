#include "configc.h"

/*
 * TODO: Check for overflow?
 */
void *xmalloc(size_t size)
{
	void *d = NULL;

	d = malloc(size);
	if (!d)
		die("OutOfMemory");

	memset(d, 0, size);
	return d;
}

inline int file_exists(const char *path)
{
	struct stat sb = { 0 };

	return stat(path, &sb) == 0;
}

inline int file_stat(const char *path, struct stat *sb)
{
	if (sb == NULL)
		sb = xmalloc(sizeof(struct stat));
	return stat(path, sb) == 0;
}

int xopen(const char *path, unsigned int flags, mode_t mode)
{
	return open(path, flags, mode);
}

/**
 * Return true when the err (coming from errno) are a I/O that could
 * be retryeable.
 */
static int is_retryable_io(int err)
{
	switch (err) {
	case EINTR:
		return true;
	}
	return false;
}

ssize_t xread(int fd, void *buf, size_t len)
{
	int retries = 0;
	ssize_t nread = 0;

	/* max IO SIZE? */
	if (len > MAX_IO_SIZE)
		return -ERR_MAX_IO_SIZE;

	do {
		nread = read(fd, buf, len);

		if (nread < 0 && is_retryable_io(nread)) {
			if (retries++ > MAX_IO_RETRY)
				return -ERR_IO_MAX_RETRY_REACHED;
			continue;
		}
		break;
	} while (true);

	return nread;
}

ssize_t xwrite(int fd, const void *buf, size_t len)
{
	int retries = 0;
	ssize_t nwrite = 0;

	if (len > MAX_IO_SIZE)
		return -ERR_MAX_IO_SIZE;

	do {
		nwrite = xwrite(fd, buf, len);

		if (nwrite < 0 && is_retryable_io(nwrite)) {
			if (retries++ > MAX_IO_RETRY)
				return -ERR_IO_MAX_RETRY_REACHED;
			continue;
		}
		break;
	} while (true);

	return nwrite;
}

char *read_whole_file(const char *path, int *rc)
{
	int fd;
	struct stat sb;
	char *file_content = NULL;

	if (!file_stat(path, &sb)) {
		perror("Unable to find file");
		*rc = -ERR_UNABLE_OPEN_FILE;
		return NULL;
	}

	fd = xopen(path, O_RDONLY, 0);
	if (fd < 0) {
		perror("Unable to open file");
		*rc = -ERR_UNABLE_OPEN_FILE;
		return NULL;
	}

	/*
	 * TODO: For now, we allocate all the content of file in memory.
	 *
	 * Obviously, this doesn't scale, so we'll have to figure out
	 * how to mmap files later and `diff()` them with a proper
	 * diff algorithm that can also handles binary files.
	 */
	file_content = xmalloc(sizeof(char) * (sb.st_size + 1));
	if (read(fd, file_content, sb.st_size) < 0) {
		free(file_content);

		*rc = -ERR_UNABLE_READ_FILE;
		return NULL;
	}
	file_content[sb.st_size] = '\0';

	*rc = 0;
	return file_content;
}
