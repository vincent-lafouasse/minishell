#include "t_redir_list.h"

const char *redir_kind_repr(t_redir_kind kind)
{
	if (kind == FROM_FILE)
		return "<";
	else if (kind == INTO_FILE)
		return ">";
	else if (kind == APPEND_INTO_FILE)
		return ">>";
	else if (kind == HERE_DOCUMENT)
		return "<<";
	return "Unknown redirection: ";
}
