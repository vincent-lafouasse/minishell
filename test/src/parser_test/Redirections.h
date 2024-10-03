#pragma once

#include <vector>

extern "C"
{
#include "parse/t_command/t_command.h"
#include "redirection/t_redir_list/t_redir_list.h"
};

struct Redirections {
	t_redir_list* self;

	Redirections(const std::vector<t_redirect>& redirs);
	Redirections(const t_simple* simple);
	Redirections(const t_subshell* subshell);
	t_redir_list* get() const;
	bool operator==(const Redirections& other) const;
	bool operator!=(const Redirections& other) const;
};

t_redirect HereDoc(const char* end);
t_redirect FromFile(const char* file);
t_redirect IntoFile(const char* file);
t_redirect AppendIntoFile(const char* file);
