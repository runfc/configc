#include "configc.h"
#include "sysctl.h"

static char *get_fullpath(const char *syskey)
{
	unsigned int i = 0;
	const char *sysproc_root = SYSPROC_ROOT;
	size_t keylen = strlen(syskey);
	size_t baselen = strlen(sysproc_root);
	char *buf = xmalloc(sizeof(char) * (baselen + keylen + 1));

	for (i = 0; i < baselen; i++)
		buf[i] = sysproc_root[i];

	for (i = 0; i < keylen; i++) {
		char c = syskey[i];

		if (c != '.')
			buf[baselen + i] = c;
		else
			buf[baselen + i] = '/';
	}
	return buf;
}

struct sysctl_t *sysctl_init(const char *key, const char *value)
{
	char *path = NULL;
	struct sysctl_t *sysctl = NULL;

	path = get_fullpath(key);
	if (!path)
		return NULL;

	sysctl = xmalloc(sizeof(struct sysctl_t));
	sysctl->key = key;
	sysctl->desired = value;
	sysctl->act_value = NULL;
	sysctl->path = path;
	return sysctl;
}

/*
 * Probe the system to get the value of sysstat configuration
 */
int sysctl_get(struct sysctl_t *sysctl)
{
	int fd, n;
	int rc = 0;
	char *value = NULL;
	char *buf = xmalloc(sizeof(char) * 64);

	if (sysctl->path == NULL)
		die("BUG: Cannot call sysctl_get() with a null sysctl->path");

	fd = xopen(sysctl->path, O_RDONLY, 0);
	if (fd < 0)
		return -ERR_UNABLE_OPEN_FILE;

	n = read(fd, buf, 512);
	if (n < 0)
	        return -ERR_UNABLE_READ_FILE;

	if (n < 0 || n > 1024)
		die("exceeed the max number of alloc() of this program, annoying I know!");

	value = xmalloc(sizeof(char) * (n + 1));
	for (int i = 0; i < n; i++) {
		char c = buf[i];

		if (c && !isspace(c))
			value[i] = buf[i];
	}

	sysctl->act_value = value;
	free(buf);
	return rc;
}

/*
 * Shows the diff between actual vs desired state
 */
int sysctl_diff(struct sysctl_t* sysctl)
{
	if (sysctl == NULL)
		die("BUG: Cannot call sysctl_diff() with a null sysctl_t");
	if (sysctl->desired == NULL)
		die("BUG: Cannot call sysctl_apply() with a null sysctl.desired");
	if (sysctl->act_value == NULL)
		die("BUG: Cannot call sysctl_apply() with a null sysctl.act_value, forgot to call sysctl_get()?");

	return strcmp(sysctl->desired, sysctl->act_value);
}

/*
 * Attempt to apply the configuration
 */
int sysctl_apply(struct sysctl_t* sysctl)
{
	int fd = 0;

	if (sysctl == NULL)
		die("BUG: Cannot call sysctl_apply() with a null sysctl_t");
	if (sysctl->desired == NULL)
		die("BUG: Cannot call sysctl_apply() with a null sysctl.desired");
	if (sysctl->act_value == NULL)
		die("BUG: Cannot call sysctl_apply() with a null sysctl.act_value, forgot to call sysctl_get()?");

	fd = xopen(sysctl->path, O_WRONLY, 0);
	if (fd < 0)
		return -ERR_UNABLE_OPEN_FILE;

	if (write(fd, sysctl->desired, strlen(sysctl->desired)) < 0)
		return -ERR_UNABLE_WRITE_FILE;
	close(fd);

	/* do another get to ensure apply worked */
	sysctl_get(sysctl);
	return 0;
}

void sysctl_free(struct sysctl_t *sysctl)
{
	if (!sysctl)
		return;

	if (sysctl->path)
		free(sysctl->path);
	if (sysctl->act_value)
		free(sysctl->act_value);
	if (sysctl)
		free(sysctl);
}
