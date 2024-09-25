maybe_cmd_suffix -> ε
First+:  $ AND PIPE ')' OR 

maybe_cmd_suffix -> cmd_suffix 
First+: '<' '>>' '>' WORD '<<' 

complete_command_rest -> OR pipeline complete_command_rest 
First+: OR 

complete_command_rest -> ε
First+: ')' $  

complete_command_rest -> AND pipeline complete_command_rest 
First+: AND 

cmd_prefix_precedes -> ε
First+: OR ')' AND PIPE  $ 

cmd_prefix_precedes -> WORD maybe_cmd_suffix 
First+: WORD 

command -> simple_command 
First+: '<' '>' WORD '<<' '>>' 

command -> subshell subshell_precedes 
First+: '(' 

subshell_precedes -> redirect_list 
First+: '<' '>>' '>' '<<' 

subshell_precedes -> ε
First+:  PIPE AND ')' $ OR 

io_file -> '>' filename 
First+: '>' 

io_file -> '>>' filename 
First+: '>>' 

io_file -> '<' filename 
First+: '<' 

cmd_suffix_rest -> WORD cmd_suffix_rest 
First+: WORD 

cmd_suffix_rest -> ε
First+:  ')' OR $ AND PIPE 

cmd_suffix_rest -> io_redirect cmd_suffix_rest 
First+: '>' '<' '>>' '<<' 

pipeline_rest -> ε
First+: AND $  OR ')' 

pipeline_rest -> PIPE command pipeline_rest 
First+: PIPE 

redirect_list_rest -> io_redirect redirect_list_rest 
First+: '>' '<' '>>' '<<' 

redirect_list_rest -> ε
First+: PIPE AND  $ ')' OR 

simple_command -> WORD maybe_cmd_suffix 
First+: WORD 

simple_command -> cmd_prefix cmd_prefix_precedes 
First+: '>>' '>' '<<' '<' 

cmd_suffix -> WORD cmd_suffix_rest 
First+: WORD 

cmd_suffix -> io_redirect cmd_suffix_rest 
First+: '>' '<' '>>' '<<' 

io_redirect -> io_here 
First+: '<<' 

io_redirect -> io_file 
First+: '>' '<' '>>' 

cmd_prefix_rest -> io_redirect cmd_prefix_rest 
First+: '>' '<' '>>' '<<' 

cmd_prefix_rest -> ε
First+: PIPE ')' AND $ OR  WORD 
