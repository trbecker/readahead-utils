%{
#include "parser.h"
#include "parser_util.h"
#define yyterminate() return END_CONFIG
%}
%option noyywrap
%%
default		{ return DEFAULT; }
mountpoint	{ return MOUNTPOINT; }
fstype		{ return FSTYPE; }
readahead	{ return READAHEAD; }
[ \t]		;
\n		{ return ENDL; }
[0-9]+		{ yylval.ival = atoi(yytext); return INT; }
[a-zA-Z0-9/]+	{ yylval.sval = strdup(yytext); return STRING; }
=		{ return EQ; }
%%
