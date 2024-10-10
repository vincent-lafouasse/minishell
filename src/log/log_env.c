#include "log.h"
#include <stdio.h>

void log_env(const t_env *env)
{
	t_env_entry entry;
	printf("environment:\n");
	for (; env != NULL; env = env->next)
	{
		entry = env->entry;
		printf("  > %s = %s\n", entry.key, entry.value);
	}
}
