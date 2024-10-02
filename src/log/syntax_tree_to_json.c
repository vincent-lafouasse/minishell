#include "log.h"
#include "parse/t_command/t_command.h"
#include "redirection/t_redir_list/t_redir_list.h"

#include <stdio.h>

static void print_simple(t_simple *simple);
static void print_cmd(t_command other);

static const char *command_type_repr(t_command_type type)
{
    if (type == PIPELINE_CMD)
        return "Pipeline";
    else if (type == SUBSHELL_CMD)
        return "Subshell";
    else if (type == SIMPLE_CMD)
        return "Simple";
    else if (type == CONDITIONAL_CMD)
        return "Conditional";
    return NULL;
}

static void print_word_list(const t_word_list *wl)
{
    printf("{");

    printf("\"text\": { \"name\": \"Word list\"},");

    printf("\"children\": [");

    while (wl)
    {
        printf("{");
        printf("\"text\": { \"name\": \"%s\"}", wl->contents);
        printf("}");
        if (wl->next != NULL)
            printf(",");
        wl = wl->next;
    }

    printf("]");

    printf("}");
}

static void print_redir_list(const t_redir_list *rdl)
{
    printf("{");

    printf("\"text\": { \"name\": \"Redir list\"},");

    printf("\"children\": [");

    while (rdl)
    {
        printf("{");
        char *word = rdl->redirect.kind == HERE_DOCUMENT ?
            rdl->redirect.doc.here_doc_eof :
            rdl->redirect.filename;
        const char *repr = redir_kind_repr(rdl->redirect.kind);
        printf("\"text\": { \"name\": \"%s %s\"}", repr, word);
        printf("}");
        if (rdl->next != NULL)
            printf(",");
        rdl = rdl->next;
    }

    printf("]");

    printf("}");
}

static void print_simple(t_simple *simple)
{
    printf("{");

    printf("\"text\": { \"name\": \"Simple\"},");

    //printf("\"HTMLclass\": \"terminal-symbol\",");
    printf("\"children\": [");
    print_word_list(simple->words);
    printf(",");
    print_redir_list(simple->redirections);
    printf("]");

    printf("}");
}

static void print_subshell(t_subshell *subshell)
{
    printf("{");

    printf("\"text\": { \"name\": \"Subshell\"},");

    printf("\"children\": [");
    print_cmd(subshell->cmd);
    printf(",");
    print_redir_list(subshell->redirections);
    printf("]");

    printf("}");
}

static void print_cmd(t_command other)
{
    if (other.type == SIMPLE_CMD)
        return print_simple(other.simple);
    if (other.type == SUBSHELL_CMD)
        return print_subshell(other.subshell);
    if (other.any == NULL)
    {
        printf("{");
        printf("\"text\": { \"name\": \"Complete Command\"},");
        printf("}");
        return ;
    }

    printf("{");

    printf("\"text\": { \"name\": \"%s\" },", command_type_repr(other.type));

    printf("\"collapsable\": true,");

    printf("\"children\": [");

    if (other.type == PIPELINE_CMD)
    {
        print_cmd(other.pipeline->first);
        printf(",");
        print_cmd(other.pipeline->second);
    }
    else if (other.type == CONDITIONAL_CMD)
        print_cmd(other.subshell->cmd);

    printf("]");

    printf("}");
}

void syntax_tree_to_json(t_command tree)
{
    printf("{");

    printf("\"chart\": { \"container\": \"#tree-simple\" },");

    printf("\"nodeStructure\": ");

    print_cmd(tree);

    printf("}\n");
}
