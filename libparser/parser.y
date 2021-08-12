%{
#include <stdio.h>
#include "parser_util.h"
#include "parser.h"
#include "libparser.h"

extern int yylex();
extern int yyparse();
extern FILE *yyin;
void yyerror(const char *s);

extern struct config_entry *config_entry_new();

struct config_entry *current;
%}

%union {
	char *sval;
	int ival;
}

%token <sval> STRING
%token <ival> INT
%token EQ
%token ENDL
%token DEFAULT
%token MOUNTPOINT
%token FSTYPE
%token READAHEAD
%token END_CONFIG 0

%%
config:
	lines
lines:
	lines line | line
line:
	tokens endls {
		struct config_entry *new = config_entry_new();
		list_add(&current->list, &new->list);
		current = new;		
	}


tokens:
	tokens token | token

token:
	default | pair

default:
	DEFAULT

pair:
	MOUNTPOINT EQ STRING	{ current->mountpoint = $3; }
	| FSTYPE EQ STRING	{ current->fstype = $3; }
	| READAHEAD EQ INT	{ current->readahead = $3; }

endls: 
	endls ENDL | ENDL

%%
int parse_config(const char *filename, struct list_head *list)
{
	yyin = fopen(filename, "r");
	current = config_entry_new();
	list_add(list, &current->list);

	yyparse();

	/* The parser will create an empty entry that need to be eliminated */
	list_del(&current->list);
	free(current);
	fclose(yyin);
}

void yyerror(const char *s) {
	printf("Parse error %s\n", s);
}
