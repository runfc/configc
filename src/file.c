#include "configc.h"
#include "file.h"

struct file_t *file_init(const char *path, const char *content, mode_t mode, unsigned int opts)
{
	struct file_t *file = NULL;

	file = xmalloc(sizeof(struct file_t));
	file->path = (char *)path;
	file->desired = (char *)content;
	file->mode = mode;
	file->act_value = NULL;
	file->opts = opts;
	return file;
}

void file_free(struct file_t *file)
{
	if (!file) /* unlikely */
		return;
	if (file->act_value)
		free(file->act_value);
	if (file->path)
		free(file->path);
	if (file->desired)
		free(file->desired);
	free(file);
}

static void check_correctness(struct file_t *file)
{
	if (!file)
		BUG("BUG: Cannot call file_get() with a null *file");
	if (!file->path)
		BUG("BUG: Cannot call file_get() with a null *file->path");
}

int file_get(struct file_t *file)
{
	int rc = 0;
	char *act_value = NULL;

	check_correctness(file);

	if (!file_exists(file->path))
		return 0;

	if (file->opts & FILE_OPT_LAZY) {
		/*
		 * in LAZY mode, we would only bother reading the file
		 * content during the diff(), so we delay this operation
		 * as much as possible.
		 *
		 * A open() is issued, in order to check if it can at
		 * least read the file, if it exists.
		 */
		int fd;

		if ((fd = open(file->path, O_RDONLY, 0) < 0)) {
			perror("[configc file] Unable to open file");
			return -ERR_UNABLE_OPEN_FILE;
		}
		close(fd);
		return 0;
	}

        act_value = read_whole_file(file->path, &rc);
	if (rc < 0 || !act_value) {
		free(file->act_value);
		return rc;
	}

	file->act_value = act_value;
	return 0;
}

int file_diff(struct file_t *file)
{
	struct stat sb;

	check_correctness(file);

	if (!file_exists(file->path))
		return 1;

	if (file_stat(file->path, &sb) && (file->mode & sb.st_mode) != file->mode)
		return 1;

	if (file->opts & FILE_OPT_LAZY) {
		int rc = 0;

	        file->act_value = read_whole_file(file->path, &rc);
		if (rc < 0)
			return rc;
	}
	return strcmp(file->desired, file->act_value);
}

/*
 * Returns a heap-allocated string where base + new are concatenated
 * together
 */
static char *strbuf_append(const char *base, const char *new)
{
	char *concated = NULL;
	size_t base_length = strlen(base);
	size_t new_length = strlen(new);

	concated = xmalloc(sizeof(char) + (base_length + new_length + 1));

	memcpy(concated, base, base_length);
	memcpy(concated + base_length, new, new_length);
	concated[base_length + new_length + 1] = '\0';
	return concated;
}

int file_apply(struct file_t *file)
{
	int err = 0;
	int fd = 0;
	char *tempfile;

	check_correctness(file);

	tempfile = strbuf_append(file->path, ".configc");
	fd = xopen(tempfile, O_CREAT | O_WRONLY, file->mode);
	if (fd < 0) {
		err = -ERR_UNABLE_OPEN_FILE;
		goto cleanup;
	}

	if (write(fd, file->desired, strlen(file->desired)) < 0) {
		err = -ERR_UNABLE_WRITE_FILE;
		goto cleanup;
	}

	if (rename(tempfile, file->path) < 0) {
		err = -ERR_UNABLE_WRITE_FILE;
		goto cleanup;
	}

	/* do another get to ensure apply worked */
	file_get(file);

 cleanup:
	free(tempfile);
	if (fd > 0)
		close(fd);
	return err;
}
