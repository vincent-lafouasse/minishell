#include "file_properties.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

t_file_properties get_file_properties(const char *path)
{
	struct stat stat_info;
	t_file_properties p;

	p = (t_file_properties){.exists = false, 0};
	if (stat(path, &stat_info) < 0)
		return (p);
	p.exists = true;
	if (!S_ISDIR(stat_info.st_mode))
	{
		p.is_executable = access(path, X_OK) == 0;
		p.is_readable = access(path, R_OK) == 0;
	}
	else
		p.is_a_directory = true;
	return (p);
}
