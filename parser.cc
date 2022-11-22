#include "parser.h"


struct InstructionNode * parse_generate_intermediate_representation()
{
    printf("program executed!\n");
    
    readAndPrintAllInput();
    return NULL;
}

// Debugging function - do not use in final submission.
void readAndPrintAllInput(){
    Token t;

    // get a token
    t = lexer.GetToken();

    // while end of input is not reached
    // Print the contents of input token list
    while (t.token_type != END_OF_FILE) 
    {
        t.Print();         	// print token
        t = lexer.GetToken();	// and get another one
    }
}
