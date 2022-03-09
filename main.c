#include "my_util.h"
#include "Parser.h"
#include "Lexer.h"

#include <stdio.h>

int yyparse();

int main() {
    yyparse();
    return 0;
}