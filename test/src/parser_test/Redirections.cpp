#include "Redirections.h"

Redirections::Redirections(const std::vector<t_redirect>& redirs) : self(nullptr)
{
	for (const t_redirect& redir: redirs)
		rdl_push_back(&self, redir);
}

Redirections::Redirections(const t_simple* simple) : self(nullptr)
{
	t_redir_list* redirs = simple->redirections;

	while (redirs)
	{
		rdl_push_back(&self, redirs->redirect);
		redirs = redirs->next;
	}
}

Redirections::Redirections(const t_subshell* subshell)
{
	t_redir_list* redirs = subshell->redirections;

	while (redirs)
	{
		rdl_push_back(&self, redirs->redirect);
		redirs = redirs->next;
	}
}


t_redir_list* Redirections::get() const
{
	return this->self;
}

bool Redirections::operator==(const Redirections& other) const
{
	t_redir_list* rhs = this->get();
	t_redir_list* lhs = other.get();

	while (rhs && lhs)
	{
		t_redirect a = rhs->redirect;
		t_redirect b = lhs->redirect;

		if (a.kind != b.kind)
			return false;
		if (a.kind == HERE_DOCUMENT && strcmp(a.doc.here_doc_eof, b.doc.here_doc_eof) != 0)
			return false;
		if (a.kind != HERE_DOCUMENT && strcmp(a.filename, b.filename) != 0)
			return false;
		rhs = rhs->next;
		lhs = lhs->next;
	}

	return (!rhs && !lhs);
}

bool Redirections::operator!=(const Redirections& other) const
{
	return !(*this == other);
}

t_redirect HereDoc(const char* end)
{
	t_here_doc hd = (t_here_doc){.contents = nullptr, .here_doc_eof = (char*)end};
	return (t_redirect){.kind = HERE_DOCUMENT, .doc = hd};
}
t_redirect FromFile(const char* file)
{
	return (t_redirect){.kind = FROM_FILE, .filename = (char*)file};
}
t_redirect IntoFile(const char* file)
{
	return (t_redirect){.kind = INTO_FILE, .filename = (char*)file};
}
t_redirect AppendIntoFile(const char* file)
{
	return (t_redirect){.kind = APPEND_INTO_FILE, .filename = (char*)file};
}
