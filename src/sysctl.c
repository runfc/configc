#include "configc.h"
#include "sysctl.h"

#define MAX_SYSCTL_BUF_SIZE 256

static void get_sysctl_fullpath(char *buf, const char *key)
{
	const char *root = SYSPROC_ROOT;
	size_t root_len = strlen(SYSPROC_ROOT);
	size_t key_len = strlen(key);

	memcpy((char *)buf, root, root_len);

	/*
	 * Copy each char from key to buf, replacing any "." with "/"
	 */
	for (size_t i = 0; i < key_len; i++) {
		char ch = key[i];

		if (ch == '.')
			buf[root_len + i] = '/';
		else
			buf[root_len + i] = ch;
	}
}

int sysctl_diff(struct sysctl_t *sysctl)
{
	int fd;
	char buf[MAX_SYSCTL_BUF_SIZE] = { 0 };
	char fullpath[MAX_SYSCTL_BUF_SIZE * 2] = { 0 };

	if (!sysctl)
		BUG("[VIOLATION] Cannot called sysctl_diff() with a null pointer");

	get_sysctl_fullpath(fullpath, sysctl->key);
	fd = open(fullpath, O_RDONLY);
	if (fd < 0)
		return -errno;

	if (read(fd, buf, MAX_SYSCTL_BUF_SIZE) < 0) {
		close(fd);
		return -errno;
	}

	close(fd);
	return strncmp(sysctl->desired_value, buf, MAX_SYSCTL_BUF_SIZE) != 0;
}

int sysctl_apply(struct sysctl_t *sysctl)
{
	int fd;
	char fullpath[MAX_SYSCTL_BUF_SIZE * 2] = { 0 };

	if (!sysctl)
		BUG("[VIOLATION] Cannot called sysctl_apply() with a null pointer");

	get_sysctl_fullpath(fullpath, sysctl->key);
	fd = open(fullpath, O_WRONLY);
	if (fd < 0)
		return -errno;

	if (write(fd, sysctl->desired_value, strlen(sysctl->desired_value)) < 0) {
		close(fd);
		return -errno;
	}

	close(fd);
	return 0;
}
