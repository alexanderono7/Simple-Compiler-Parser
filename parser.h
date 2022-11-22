#ifndef __PARSER_H__
#define __PARSER_H__

#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <vector>
#include <string>
#include "execute.h"
#include "lexer.h"


LexicalAnalyzer lexer;  

// maybe don't use this?
class Parser {
    public:
    private:
};

void readAndPrintAllInput();

#endif