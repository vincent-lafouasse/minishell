## common sets
(1) AND OR PIPE ')' $
(2) < << > >>

## Complete command rest
complete_command_rest -> OR pipeline complete_command_rest 
First+: OR 

complete_command_rest -> AND pipeline complete_command_rest 
First+: AND 

complete_command_rest -> ε
First+: ')' $  

## Pipeline rest

pipeline_rest -> PIPE command pipeline_rest 
First+: PIPE 

pipeline_rest -> ε
First+: AND OR ')' $ (1 - PIPE)

## Command

command -> simple_command 
First+: WORD '<' '>' '<<' '>>' (2 + WORD)

command -> subshell subshell_precedes 
First+: '(' 

## Subshell precedes

subshell_precedes -> redirect_list 
First+: '<' '>>' '>' '<<' (2)

subshell_precedes -> ε
First+: AND OR PIPE ')' $ (1)

## Simple command

simple_command -> WORD maybe_cmd_suffix 
First+: WORD 

simple_command -> cmd_prefix cmd_prefix_precedes 
First+: '>>' '>' '<<' '<' (2)

## Maybe command suffix

maybe_cmd_suffix -> cmd_suffix 
First+: WORD '<' '>>' '>' '<<' (2)

maybe_cmd_suffix -> ε
First+: AND OR PIPE ')' $ (1)

## Command prefix precedes

cmd_prefix_precedes -> WORD maybe_cmd_suffix 
First+: WORD

cmd_prefix_precedes -> ε
First+: AND OR PIPE ')' $ (1)

## Command prefix rest

cmd_prefix_rest -> io_redirect cmd_prefix_rest 
First+: '>' '<' '>>' '<<' (2)

cmd_prefix_rest -> ε
First+: WORD AND OR PIPE ')' $ (1 + WORD)

## Command suffix

cmd_suffix -> WORD cmd_suffix_rest 
First+: WORD 

cmd_suffix -> io_redirect cmd_suffix_rest 
First+: '>' '<' '>>' '<<' (2)

## Command suffix rest

cmd_suffix_rest -> WORD cmd_suffix_rest 
First+: WORD 

cmd_suffix_rest -> io_redirect cmd_suffix_rest 
First+: '>' '<' '>>' '<<' (2)

cmd_suffix_rest -> ε
First+: AND OR PIPE ')' $ (1)

## redirect list rest

redirect_list_rest -> io_redirect redirect_list_rest 
First+: '>' '<' '>>' '<<' (2)

redirect_list_rest -> ε
First+: AND OR PIPE ')' $ (1)

## IO Redirect

io_redirect -> io_here 
First+: '<<' 

io_redirect -> io_file 
First+: '>' '<' '>>' 

## IO File

io_file -> '>' filename 
First+: '>' 

io_file -> '>>' filename 
First+: '>>' 

io_file -> '<' filename 
First+: '<' 
