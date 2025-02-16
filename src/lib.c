#include "configc.h"

inline int file_exists(const char *path)
{
	struct stat sb = { 0 };

	return stat(path, &sb) == 0;
}
