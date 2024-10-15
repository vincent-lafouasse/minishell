#include "Redirections.h"
#include "io/t_redir_list/t_redir_list.h"
#include <cstring>

static t_redirect redirect_copy(t_redirect redir)
{
    if (redir.kind == HERE_DOCUMENT)
    {
        t_here_doc here_document = (t_here_doc){
            .here_doc_eof = strdup(redir.doc.here_doc_eof)
        };
        return (t_redirect){.kind = HERE_DOCUMENT, .doc = here_document};
    }
    else
        return (t_redirect){.kind = redir.kind, .filename = strdup(redir.filename)};
}

static bool redirect_eq(t_redirect a, t_redirect b)
{
    if (a.kind != b.kind)
        return false;
    if (a.kind == HERE_DOCUMENT)
        return strcmp(a.doc.here_doc_eof, b.doc.here_doc_eof) == 0;
    else
        return strcmp(a.filename, b.filename) == 0;
}

Redirections::Redirections(const std::vector<t_redirect> &redirs)
    : redirections(redirs) {}

Redirections::Redirections(const t_simple *simple) : redirections(std::vector<t_redirect>{})
{
    t_redir_list *redirs = simple->redirections;

    while (redirs)
    {
        this->redirections.push_back(redirs->redirect);
        redirs = redirs->next;
    }
}

Redirections::Redirections(const t_subshell* subshell): redirections(std::vector<t_redirect>{})
{
    t_redir_list *redirs = subshell->redirections;

    while (redirs)
    {
        this->redirections.push_back(redirs->redirect);
        redirs = redirs->next;
    }
}

t_redir_list *Redirections::to_list() const {
    t_redir_list *out = nullptr;

    for (const auto &redir : redirections)
    {
        rdl_push_back(&out, redirect_copy(redir));
    }

    return (out);
}

bool Redirections::operator==(const Redirections &other) const
{
    if (this->redirections.size() != other.redirections.size())
        return false;

    for (size_t i = 0; i < this->redirections.size(); i++)
    {
        if (!redirect_eq(this->redirections[i], other.redirections[i]))
            return (false);
    }

    return (true);
}

bool Redirections::operator!=(const Redirections &other) const
{
    return !(*this == other);
}

t_redirect HereDoc(const char *end)
{
    t_here_doc hd =
        (t_here_doc){.contents = nullptr, .here_doc_eof = (char *)end};
    return (t_redirect){.kind = HERE_DOCUMENT, .doc = hd};
}
t_redirect FromFile(const char *file)
{
    return (t_redirect){.kind = FROM_FILE, .filename = (char *)file};
}
t_redirect IntoFile(const char *file)
{
    return (t_redirect){.kind = INTO_FILE, .filename = (char *)file};
}
t_redirect AppendIntoFile(const char *file)
{
    return (t_redirect){.kind = APPEND_INTO_FILE, .filename = (char *)file};
}

Redirections::~Redirections() {}
