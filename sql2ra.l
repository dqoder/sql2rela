%{

#include "my_util.h"
#include "Parser.h" 

void yyerror (char *s);
int yylex();

%}

%option outfile="Lexer.c" header-file="Lexer.h"

%%

[ \r\n\t]* {; /*skip blanks*/ }
[-+]?([0-9]*\.[0-9]+|[0-9]+\.[0-9]*) {yylval.num_float= atof(yytext);return real; }
[-+]?[0-9]+        { yylval.num_int=atoi(yytext); return integer;}

(?i:FROM)          return from;
(?i:WHERE)         return where;
(?i:SELECT)        return select_k;
(?i:ON)            return on;

(?i:"INNER JOIN")  return join;
(?i:"LEFT JOIN")   return join;
(?i:"RIGHT JOIN")  return join;
(?i:"CROSS JOIN")  return join;

(?i:JOIN)          return join;

(?i:UNION)         { yylval.sopr = UNION; return set_union;}
(?i:INTERSECT)     { yylval.sopr = INTER; return set_intersect;}
(?i:MINUS)         { yylval.sopr = MINUS; return set_minus;}

(?i:AND)           { yylval.lopr = AND; return log_and; }
(?i:OR)            { yylval.lopr = OR ; return log_or; }

[a-zA-Z][a-zA-Z_0-9]* return identifier;
[-+*/;.(),]           return yytext[0];

"="                { yylval.ropr = EQ; return eq;}
">="               { yylval.ropr = GE; return ge;}
"<="               { yylval.ropr = LE; return le;}
"<"                { yylval.ropr = LT; return lt;}
">"                { yylval.ropr = GT; return gt;}
['"][a-zA-Z]*['"]  return string;
%%

int yywrap(void) {return 1;}
