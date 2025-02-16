#include "configc.h"
#include "file.h"

#define MAX_FILE_BUF_SIZE 2048

static int check_file_diff(struct file_t *file)
{
	int rc = 0;
	int fd;
	int nread;
	char buf[MAX_FILE_BUF_SIZE] = { 0 };

	fd = open(file->path, O_RDONLY, 0);
	if (fd < 0)
		return -errno;

	do {
		nread = read(fd, buf, MAX_FILE_BUF_SIZE);
		if (nread == 0) {
			rc = 0;
			break;
		} else if (nread < 0) {
			rc = -errno;
			break;
		}

		/*
		 * As we read MAX_FILE_BUF_SIZE at a time, we should ensure that it is also
		 * the upper boundary for comparasion.  Not only this optmize the codes, but
		 * most importantly it provide more safety properties.
		 */
		if (strncmp(buf, file->desired_value, MAX_FILE_BUF_SIZE)) {
			rc = 1;
			break;
		}

	} while (true);

	close(fd);
	return rc;
}

int file_diff(struct file_t *file)
{
	struct stat sb = { 0 };

	if (!file)
		BUG("[VIOLATION] Cannot call file_diff() with a null pointer");

	if (stat(file->path, &sb) < 0) {
		if (errno == ENOENT) {
			/*
			 * When a file does not exists, we should return there's a
			 * a diff() between desired and actual state
			 */
			return 1;
		} else
			return -errno;
	}

	return check_file_diff(file);
}

#define TMPFILE_PREFIX ".konfig"
#define TMPFILE_PREFIX_SIZE 7

int file_apply(struct file_t *file)
{
	int rc = 0;
	int fd;
	char tempfile[MAX_FILE_BUF_SIZE] = { 0 };
	size_t path_len = strlen(file->path);
	size_t desired_value_len = strlen(file->desired_value);

	if (!file)
		BUG("[VIOLATION] Cannot call file_apply() with a null pointer");
	if ((path_len + 7) >= MAX_FILE_BUF_SIZE)
		BUG("File's path is too large");

	memcpy(tempfile, file->path, path_len);
	memcpy(tempfile + path_len + 1, TMPFILE_PREFIX, TMPFILE_PREFIX_SIZE);

	fd = open(tempfile, O_CREAT | O_WRONLY, file->mode);
	if (fd < 0)
		return -errno;

	if (write(fd, file->desired_value, desired_value_len) < 0 ||
	    fsync(fd) < 0 ||
	    rename(tempfile, file->path) < 0) {
		rc = -errno;
	}

	close(fd);
	return rc;
}
