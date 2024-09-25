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
First+: AND OR ')' $

## Command

command -> simple_command 
First+: WORD '<' '>' '<<' '>>' 

command -> subshell subshell_precedes 
First+: '(' 

## Subshell precedes

subshell_precedes -> redirect_list 
First+: '<' '>>' '>' '<<' 

subshell_precedes -> ε
First+: AND OR PIPE ')' $

## Simple command

simple_command -> WORD maybe_cmd_suffix 
First+: WORD 

simple_command -> cmd_prefix cmd_prefix_precedes 
First+: '>>' '>' '<<' '<' 

## Maybe command suffix

maybe_cmd_suffix -> cmd_suffix 
First+: WORD '<' '>>' '>' '<<' 

maybe_cmd_suffix -> ε
First+: AND OR PIPE ')' $

## Command prefix precedes

cmd_prefix_precedes -> WORD maybe_cmd_suffix 
First+: WORD

cmd_prefix_precedes -> ε
First+: AND OR PIPE ')' $

## Command prefix rest

cmd_prefix_rest -> io_redirect cmd_prefix_rest 
First+: '>' '<' '>>' '<<' 

cmd_prefix_rest -> ε
First+: WORD AND OR PIPE ')' $

## Command suffix

cmd_suffix -> WORD cmd_suffix_rest 
First+: WORD 

cmd_suffix -> io_redirect cmd_suffix_rest 
First+: '>' '<' '>>' '<<' 

## Command suffix rest

cmd_suffix_rest -> WORD cmd_suffix_rest 
First+: WORD 

cmd_suffix_rest -> io_redirect cmd_suffix_rest 
First+: '>' '<' '>>' '<<' 

cmd_suffix_rest -> ε
First+: AND OR PIPE ')' $

## redirect list rest

redirect_list_rest -> io_redirect redirect_list_rest 
First+: '>' '<' '>>' '<<' 

redirect_list_rest -> ε
First+: AND OR PIPE ')' $

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
