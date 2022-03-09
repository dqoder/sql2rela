%{
void yyerror(char *s);
int yylex();

#include "my_util.h"
#include <stdio.h>
#include <stdlib.h>

#include "Parser.h"
#include "Lexer.h"


%}

/*
    we are giving values to query, and numbers only
    value of query === no. of attributes, for ex:
        select_k a, b ... = 2
        select_k a ..     = 1
        select_k * ...    = 0

        This is done because for set operations (like union) of query 
            --> no.of attributes should be same.
            --> if values of one query is 1 and other 2 
                --> incompatible for set operation 
                    --> generate error.

    value of number (float(real) or integer) is taken,
        (no reason at all for now)
*/

%output "Parser.c"
%defines "Parser.h"

%union {
    int num_int;
    float num_float;
    logopr lopr;
    setopr sopr;
    relopr ropr;
    p_type* ptype;
}

%start S // query with set operations


%token select_k // select keyword
%token where
%token from  
%token join
%token on

%token identifier

%token <sopr> set_union
%token <sopr> set_minus
%token <sopr> set_intersect

%token <lopr> log_and
%token <lopr> log_or

%token string

%token <ropr> eq
%token <ropr> le
%token <ropr> ge 
%token <ropr> lt 
%token <ropr> gt

%token <num_int> integer
%token <num_float> real


%type <ptype> query s_query select_clause attributes attribute where_clause
%type <ptype> from_where_clause join_clause
%type <ptype> condition condition_join conditions conditions_join 
%type <ptype> on_clause mattribute string tables table valmat valat

%type <sopr> set_op
%type <lopr> log_opr
%type <ropr> rel_opr


%type <num_int> expr number 

%%
S: s_query         {printf("converted: %s\n", $1->str);exit(EXIT_SUCCESS);}
    | s_query ';'  {printf("converted: %s\n", $1->str);exit(EXIT_SUCCESS);}
    ;

s_query: query  { $$ = $1;}
        | query set_op s_query {$$=setop($1, $2, $3);}
        ;

query: select_clause from_where_clause {$$ = createQueryClause($1,$2);}
        | '(' query ')' {$$ = $2;}
        ;

number: integer {$$ = (int)($1);}
        | real {$$ = (int)($1);}
        ;

select_clause: select_k attributes {$$ = createSelectClause($2);}
        ;

from_where_clause: from tables where_clause {$$ = createFromClause($2, $3);}
        | from join_clause where_clause {$$ = createFromClause($2,$3);}
        ;

where_clause: {$$ = createPT(0,0, NULL);}
        | where conditions {$$ = $2;}
        ;

conditions: condition {$$ = $1;}
        | condition log_opr conditions {$$ = logop($1, $2, $3);}
        ;

log_opr: log_and {;}
        | log_or {;}
        ;

rel_opr: eq | le | ge | lt | gt ;

condition: number {$$ = createPTfromNum(0,0,$1);}
        | '(' condition ')' {$$ = $2;}
        | valat rel_opr valat {$$ = relop($1, $2, $3);}
        ;

valat: expr {$$ = createPTfromNum(0,0,$1);}
        | number {$$= createPTfromNum(0,0,$1);}
        | attribute {$$ = createPT(0,0,yytext);}
        | string {$$ = createPT(0,0,yytext);}
        ;

expr: '(' expr ')' {$$ = $2;}
        | number '+' expr {$$ = $1 + $3;}
        | number '-' expr {$$ = $1 - $3;}
        | number '*' expr {$$ = $1 * $3;}
        | number '/' expr {$$ = $1 / $3;}
        ;

set_op: set_intersect  {;}
        | set_union     {;}
        | set_minus     {;}
        ;

attributes: '*' {$$ = createPT(0, 1, "*");}
        | attribute {$$ = $1 ;}
        | attribute ',' attributes {$$ = concat_attr($1, $3);}
        ; 

tables: table {$$ = $1;}
        | table ',' tables {$$ = createJoinComma($1, $3);}
        ;

join_clause: table join table on_clause {$$ = createJoin($1, $3, $4);}
        | join_clause join table on_clause {$$ = createJoinClause($1, $3, $4);}
        ;

on_clause: {$$ = createPT(0,0,NULL);}
        | on conditions_join {$$ = $2;}
        ;

conditions_join: condition_join {$$ = $1;}
        | condition_join log_opr conditions_join {$$ = logop($1, $2, $3);}
        ;

condition_join: number {$$ = createPTfromNum(0,0, $1);}
        | '(' condition_join ')' {$$ = $2;}
        | valmat rel_opr valmat {$$ = relop($1, $2, $3);}
        ;

valmat: expr {$$ = createPTfromNum(0, 0, $1);}
        | number {$$ = createPTfromNum(0,0, $1);}
        | mattribute {$$ = $1;}
        | string {$$ = createPT(0, 0, yytext);}
        ;

mattribute: table '.' attribute {$$=createMatt($1, $3);}     ;

table: identifier {$$ = createPT(0,0,yytext);};
attribute: identifier {$$ = createPT(1,1,yytext);};

%%

void yyerror(char *s) {fprintf(stderr, "%s\n", s);}