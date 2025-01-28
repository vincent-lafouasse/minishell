#include <stdbool.h>
#include <sys/stat.h>

bool file_is_directory(const char *file_path)
{
	struct stat file_stats;
	if (stat(file_path, &file_stats) < 0)
		return false;
	return (S_ISDIR(file_stats.st_mode));
}
