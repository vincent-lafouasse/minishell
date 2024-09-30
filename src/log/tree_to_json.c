#include "log.h"

#include <stdio.h>

static const char* node_repr(enum e_symbol_kind kind)
{
    if (kind == PROGRAM)
        return "Program";
    if (kind == COMPLETE_COMMAND)
        return "Complete Command";
    if (kind == COMPLETE_COMMAND_REST)
        return "Complete Command'";
    if (kind == PIPELINE)
        return "Pipeline";
    if (kind == PIPELINE_REST)
        return "Pipeline'";
    if (kind == COMMAND)
        return "Command";
    if (kind == WORD_LIST)
        return "Wordlist";
    if (kind == WORD_LIST_REST)
        return "Wordlist'";
    return "???";
}

void print_terminal(const t_symbol* node);
void print_non_terminal(const t_symbol* node);

void print_node(const t_symbol* node)
{
    if (node->kind == TERMINAL)
        print_terminal(node);
    else
        print_non_terminal(node);
}

void print_terminal(const t_symbol* node)
{
    printf("{");

    printf("\"text\": { \"name\": \"");
    if (node->token.type == WORD)
        printf("%s", node->token.literal);
    else
        printf("%s", token_repr(node->token));
    printf("\"},");

    printf("\"children\": []");

    printf("}");
}

void print_non_terminal(const t_symbol* node)
{
    printf("{");

    printf("\"text\": { \"name\": \"%s\" },", node_repr(node->kind));



    printf("\"children\": [");

    for (size_t i = 0; i < node->production->len; i++)
    {
        print_node(&node->production->data[i]);
        if (i != node->production->len - 1)
            printf(",");
    }

    printf("]");

    printf("}");
}

void tree_to_json(const t_symbol *root)
{
    printf("{");

    printf("\"chart\": { \"container\": \"#tree-simple\" },");

    printf("\"nodeStructure\": ");

    print_node(root);

    printf("}\n");
}
