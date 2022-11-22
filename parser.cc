#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include "execute.h"
#include "lexer.h"


LexicalAnalyzer lexer;   // NOTE: even though this file does not peek() at or get() any
                         // tokens, the very fact that a lexer is declared
                         // requires that you provide input to the program
                         // when you execute it even if you don't add any code
                         // to execute just run ./a.out < test, where test is any
                         // test case

struct InstructionNode * parse_generate_intermediate_representation()
{
    printf("program executed!\n");
}
