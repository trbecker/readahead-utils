%{
#include "parser.h"
#define yyterminate() return END_CONFIG
%}
%option noyywrap
%%
default		{ return DEFAULT; }
mountpoint	{ return MOUNTPOINT; }
fstype		{ return FSTYPE; }
readahead	{ return READAHEAD; }
nfs4		{ yylval.sval = strdup(yytext); return FS; }
nfs		{ yylval.sval = strdup(yytext); return FS; }
[ \t]		;
\n		{ return ENDL; }
[0-9]+		{ yylval.ival = atoi(yytext); return INT; }
[a-zA-Z0-9/]+	{ yylval.sval = strdup(yytext); return STRING; }
=		{ return EQ; }
%%
