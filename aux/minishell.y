%token PIPE
%token LOGICAL
%token SIMPLE

%start command
%%
command		: PIPE
		 	| LOGICAL
			| SIMPLE
			;

PIPE		: command "|" command
LOGICAL		: command "&&" command
		 	| command "||" command
			;
