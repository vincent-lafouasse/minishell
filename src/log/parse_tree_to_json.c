/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tree_to_json.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:11:57 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 19:15:05 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "log.h"
#include <stdio.h>

static const char	*node_repr(enum e_symbol_kind kind)
{
	if (kind == SYM_COMPLETE_CMD)
		return ("Complete Command");
	if (kind == SYM_SUBSHELL)
		return ("Subshell");
	if (kind == SYM_CMD_PREFIX_REST)
		return ("Cmd Prefix'");
	if (kind == SYM_IO_REDIRECT)
		return ("Io Redirect");
	if (kind == SYM_IO_FILE)
		return ("Io File");
	if (kind == SYM_PROGRAM)
		return ("Program");
	if (kind == SYM_CMD_PREFIX_PRECEDES)
		return ("Cmd Prefix''");
	if (kind == SYM_IO_HERE)
		return ("Io Here");
	if (kind == SYM_COMPLETE_CMD_REST)
		return ("Complete Command'");
	if (kind == SYM_MAYBE_CMD_SUFFIX)
		return ("Maybe Cmd Suffix");
	if (kind == SYM_CMD_SUFFIX)
		return ("Cmd Suffix");
	if (kind == SYM_PIPELINE_REST)
		return ("Pipeline'");
	if (kind == SYM_PIPELINE)
		return ("Pipeline");
	if (kind == SYM_REDIRECT_LIST)
		return ("Redirect List");
	if (kind == SYM_SIMPLE_CMD)
		return ("Simple Command");
	if (kind == SYM_HERE_END)
		return ("Here End");
	if (kind == SYM_FILENAME)
		return ("Filename");
	if (kind == SYM_CMD_PREFIX)
		return ("Cmd Prefix");
	if (kind == SYM_COMMAND)
		return ("Command");
	if (kind == SYM_CMD_SUFFIX_REST)
		return ("Cmd Suffix'");
	if (kind == SYM_REDIRECT_LIST_REST)
		return ("Redirect List'");
	if (kind == SYM_SUBSHELL_PRECEDES)
		return ("Subshell''");
	return ("???");
}

void				print_terminal(const t_symbol *node);
void				print_non_terminal(const t_symbol *node);

void	print_node(const t_symbol *node)
{
	if (node->kind == SYM_TERMINAL)
		print_terminal(node);
	else
		print_non_terminal(node);
}

void	print_terminal(const t_symbol *node)
{
	printf("{");
	printf("\"text\": { \"name\": \"");
	if (node->token.type == WORD)
		printf("%s", node->token.literal);
	else
		printf("%s", token_repr(node->token));
	printf("\"},");
	printf("\"HTMLclass\": \"terminal-symbol\",");
	printf("\"children\": []");
	printf("}");
}

void	print_non_terminal(const t_symbol *node)
{
	size_t	i;

	i = 0;
	printf("{");
	printf("\"text\": { \"name\": \"%s\" },", node_repr(node->kind));
	printf("\"collapsable\": true,");
	printf("\"children\": [");
	while (i < node->production->len)
	{
		print_node(&node->production->data[i]);
		if (i != node->production->len - 1)
			printf(",");
		i++;
	}
	printf("]");
	printf("}");
}

void	parse_tree_to_json(const t_symbol *root)
{
	printf("{");
	printf("\"chart\": { \"container\": \"#tree-simple\" },");
	printf("\"nodeStructure\": ");
	print_node(root);
	printf("}\n");
}
