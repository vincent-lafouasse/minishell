%token osef

%start command
%%
complete_cmd	: pipe_cmd
		 		| logical_cmd
				| simple_cmd
				;

pipe_comd		: command "|" command
logical_cmd		: command "&&" command
		 		| command "||" command
				;
