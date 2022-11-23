#include "parser.h"

/* 
As stated in the project document, there shouldn't be any syntax/semantic errors in any
test cases, but I've included this so I know if my code is messing up somewhere.
*/
void raise_error(){
    readAndPrintAllInput();
    cout << "\nERROR ERROR ERROR!\n";
    cout << "\nERROR ERROR ERROR!\n";
    cout << "\nERROR ERROR ERROR!\n";
    exit(1);
}


Token expect(TokenType expected_type){
    Token t = lexer.GetToken();
    if(t.token_type != expected_type)
        raise_error();
    return t;
} 

// Equivalent to parse_program?
struct InstructionNode * parse_generate_intermediate_representation()
{
    cout << "program executed!\n"; // remove later
    // program -> var_section body inputs

    readAndPrintAllInput();
    parse_var_section();
    parse_body();
    parse_inputs();
    
    return NULL; // should return head node to intermediate code
}

void parse_var_section(){
    // var_section -> id_list SEMICOLON
    // Contains the names of variables.
    parse_id_list();
    expect(SEMICOLON);
}

void parse_id_list(){
    // id_list -> ID COMMA id_list | ID
    expect(ID);

    Token t = lexer.peek(1);
    if(t.token_type == COMMA){
        expect(COMMA);
        parse_id_list(); 
    }else if(t.token_type == SEMICOLON){
        // end of id_list and end of var_section
        return;
    }else{
        raise_error();
    }
}

void parse_body(){
    // body -> LBRACE stmt_list RBRACE
    expect(LBRACE);
    parse_stmt_list();
    expect(RBRACE);
}

InstructionNode* parse_stmt_list(){
    // stmt_list -> stmt stmt_list | stmt
    parse_stmt();
    Token t = lexer.peek(1);

    // FIRST(stmt) = {ID, "output", "input", WHILE, IF, SWITCH, FOR}
    set<TokenType> stmt_fs({ID, OUTPUT, INPUT, WHILE, IF, SWITCH, FOR});  // first set of stmt

    if(stmt_fs.find(t.token_type) != stmt_fs.end()){ // check if t.token_type is in FIRST(stmt)
        parse_stmt_list(); 
    }else if(t.token_type == RBRACE){ // is this really accurate?
        // end of statement list
        return;
    }else{
        raise_error();
    }
}

InstructionNode* parse_stmt(){
    Token t = lexer.peek(1);
    switch(t.token_type){
        case ID:
            InstructionNode* parse_assign_stmt;
            break;
        case OUTPUT:
            InstructionNode* parse_output_stmt;
            break;
        case INPUT:
            InstructionNode* parse_input_stmt;
            break;
        case WHILE:
            InstructionNode* parse_while_stmt;
            break;
        case IF:
            InstructionNode* parse_if_stmt;
            break;
        case SWITCH:
            InstructionNode* parse_switch_stmt;
            break;
        case FOR:
            InstructionNode* parse_for_stmt;
            break;
    }
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
