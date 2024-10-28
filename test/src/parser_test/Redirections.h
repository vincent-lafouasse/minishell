#pragma once

#include <vector>
#include <iostream>

extern "C"
{
#include "parse/t_command/t_command.h"
#include "io/t_redir_list/t_redir_list.h"
};

struct Redirections
{
    std::vector<t_redirect> redirections;

    Redirections(const std::vector<t_redirect> &redirs);
    Redirections(const t_simple *simple);
    Redirections(const t_subshell *subshell);
    ~Redirections();

    friend std::ostream& operator<<(std::ostream& os, const Redirections& redir)
    {
        os << "Redirections {";
        for (const auto &redir: redir.redirections)
        {
            char *lit = redir.kind != HERE_DOCUMENT ? redir.filename : redir.doc.here_doc_eof;
            os << redir_kind_repr(redir.kind) << " '" << lit << "', ";
        }
        os << "}" << std::endl;
        return (os);
    }

    t_redir_list *to_list() const;
    bool operator==(const Redirections &other) const;
    bool operator!=(const Redirections &other) const;
};

t_redirect HereDoc(const char *end);
t_redirect FromFile(const char *file);
t_redirect IntoFile(const char *file);
t_redirect AppendIntoFile(const char *file);
