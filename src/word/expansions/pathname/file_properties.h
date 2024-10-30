#ifndef FILE_PROPERTIES_H
# define FILE_PROPERTIES_H

# include <stdbool.h>

typedef struct s_file_properties {
	bool exists;
	bool is_a_directory;
	bool is_readable;
	bool is_executable;
} t_file_properties;

t_file_properties get_file_properties(const char *path);

#endif // FILE_PROPERTIES_H
