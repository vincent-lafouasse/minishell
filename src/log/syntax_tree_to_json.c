#include "log.h"
#include "parse/t_command/t_command.h"
#include "redirection/t_redir_list/t_redir_list.h"

#include <assert.h>
#include <stdio.h>

static void print_simple(t_simple *simple);
static void print_cmd(t_command other);

static void print_word_list(const t_word_list *wl)
{
    printf("{");

    printf("\"text\": { \"name\": \"Word list\"},");
    printf("\"stackChildren\": true,");

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


    printf("\"collapsable\": true,");
    printf("\"stackChildren\": true,");

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

    printf("\"collapsable\": true,");

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

    printf("\"collapsable\": true,");

    printf("\"children\": [");
    print_cmd(subshell->cmd);
    printf(",");
    print_redir_list(subshell->redirections);
    printf("]");

    printf("}");
}

static void print_pipeline(t_pipeline *pipeline)
{
    printf("{");

    printf("\"text\": { \"name\": \"Pipeline\"},");

    printf("\"collapsable\": true,");

    printf("\"children\": [");
    print_cmd(pipeline->first);
    printf(",");
    print_cmd(pipeline->second);
    printf("]");

    printf("}");
}

static void print_conditional(t_conditional *conditional)
{
    printf("{");

    const char *operator =
        conditional->operator == AND_OP ? "And" : "Or";
    printf("\"text\": { \"name\": \"Conditional (%s)\" },", operator);

    printf("\"collapsable\": true,");

    printf("\"children\": [");

    print_cmd(conditional->first);
    printf(",");
    print_cmd(conditional->second);

    printf("]");

    printf("}");
}

static void print_cmd(t_command other)
{
    assert(other.any != NULL);
    if (other.type == SIMPLE_CMD)
        return print_simple(other.simple);
    if (other.type == SUBSHELL_CMD)
        return print_subshell(other.subshell);
    if (other.type == PIPELINE_CMD)
        return print_pipeline(other.pipeline);
    if (other.type == CONDITIONAL_CMD)
        return print_conditional(other.conditional);
}

void syntax_tree_to_json(t_command tree)
{
    printf("{");

    printf("\"chart\": { \"container\": \"#tree-simple\" },");

    printf("\"nodeStructure\": ");

    print_cmd(tree);

    printf("}\n");
}
