#include "parser.h"

LexicalAnalyzer lexer;  // hopefully this won't cause issues, but I might need to combine parser.cc and other.cc into 1 file.

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
    InstructionNode* head;

    //raise_error(); // for testing/debugging
    parse_var_section();
    parse_body();
    parse_inputs();
    
    return head; // should return head node to intermediate code
}

void parse_var_section(){
    // var_section -> id_list SEMICOLON
    // Contains the names of variables.
    parse_id_list();
    expect(SEMICOLON);
}

void parse_id_list(){
    // id_list -> ID COMMA id_list | ID
    Token var = expect(ID);
    new_variable(var.lexeme);

    TokenType t = lexer.peek(1).token_type;
    if(t==COMMA){
        expect(COMMA);
        parse_id_list(); 
    }else if(t==SEMICOLON){
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
    InstructionNode* stmt = parse_stmt();
    TokenType t = lexer.peek(1).token_type;

    // FIRST(stmt) = {ID, "output", "input", WHILE, IF, SWITCH, FOR}
    set<TokenType> stmt_fs({ID, OUTPUT, INPUT, WHILE, IF, SWITCH, FOR});  // first set of stmt

    if(stmt_fs.count(t)){ // check if t.token_type is in FIRST(stmt)
        stmt->next = parse_stmt_list(); 
    }else if(t==RBRACE){ // is this really accurate?
        // end of statement list
        return NULL;
    }else{
        raise_error();
    }
    return stmt; // placeholder
}

InstructionNode* parse_stmt(){
    // I believe new instruction nodes should be instanciated here.
    InstructionNode* stmt = new InstructionNode();

    TokenType t = lexer.peek(1).token_type;
    switch(t){
        case ID:
            parse_assign_stmt(stmt);
            break;
        case OUTPUT:
            parse_output_stmt(stmt);
            break;
        case INPUT:
            parse_input_stmt(stmt);
            break;
        case WHILE:
            parse_while_stmt(stmt);
            break;
        case IF:
            parse_if_stmt(stmt);
            break;
        case SWITCH:
            parse_switch_stmt(stmt);
            break;
        case FOR:
            parse_for_stmt(stmt);
            break;
        default:
            raise_error();
            break;
    }
    return stmt; // placeholder
}

InstructionNode* parse_assign_stmt(InstructionNode* stmt){
    // assign_stmt -> ID EQUAL primary SEMICOLON | ID EQUAL expr SEMICOLON
    expect(ID);
    expect(EQUAL);

    TokenType t = lexer.peek(1).token_type;
    TokenType s = lexer.peek(2).token_type;
    // check if t is in FIRST(primary) or FIRST(expr)
    // FIRST(primary) = ID | NUM //only has 1 token
    // FIRST(expr) = ID | NUM 
    // SECOND(expr) = op
    if(t==ID or t==NUM){
        if(s==SEMICOLON){
            parse_primary();
        }else if(s==PLUS or s==MINUS or s==MULT or s==DIV){
            parse_expr();
        }else{
            raise_error();
        }
    }else{
        raise_error();
    }

    expect(SEMICOLON);
    return NULL; // placeholder
}

void parse_expr(){
    // expr -> primary op primary
    parse_primary();
    parse_op();
    parse_primary();
}

void parse_primary(){
    // primary -> ID | NUM
    Token t = lexer.peek(1);

    if(t.token_type == ID)
        expect(ID);
    else if(t.token_type == NUM)
        expect(NUM);
    else{
        raise_error();
    }
}

void parse_op(){
    // op -> PLUS | MINUS | MULT | DIV
    TokenType t = lexer.peek(1).token_type;
    switch(t){
        case PLUS:
            expect(PLUS);
            break;
        case MINUS:
            expect(MINUS);
            break;
        case MULT:
            expect(MULT);
            break;
        case DIV:
            expect(DIV);
            break;
        default:
            raise_error();
            break;
    }
}

InstructionNode* parse_output_stmt(InstructionNode* stmt){
    // output_stmt = OUTPUT ID SEMICOLON
    expect(OUTPUT);
    expect(ID);
    expect(SEMICOLON);
    return NULL; // placeholder
}

InstructionNode* parse_input_stmt(InstructionNode* stmt){
    // input_stmt = INPUT ID SEMICOLON
    expect(INPUT);
    expect(ID);
    expect(SEMICOLON);
    return NULL; // placeholder
}

InstructionNode* parse_while_stmt(InstructionNode* stmt){
    // while_stmt -> WHILE condition body
    expect(WHILE);
    parse_condition();
    parse_body();
    return NULL; // placeholder
}

InstructionNode* parse_if_stmt(InstructionNode* stmt){
    // if_stmt -> IF condition body
    expect(IF);
    parse_condition();
    parse_body();
    return NULL; // placeholder
}

void parse_condition(){
    // condition -> primary relop primary
    parse_primary();
    parse_relop();
    parse_primary();
}

void parse_relop(){
    // relop -> GREATER | LESS | NOTEQUAL
    TokenType t = lexer.peek(1).token_type;
    switch(t){
        case GREATER:
            expect(GREATER);
            break;
        case LESS:
            expect(LESS);
            break;
        case NOTEQUAL:
            expect(NOTEQUAL);
            break;
        default:
            raise_error();
            break;
    }
}

InstructionNode* parse_switch_stmt(InstructionNode* stmt){
    // switch_stmt -> SWITCH ID LBRACE case_list RBRACE
    // switch_stmt -> SWITCH ID LBRACE case_list default_case RBRACE
    expect(SWITCH);
    expect(ID);
    expect(LBRACE);
    parse_case_list();
    TokenType t = lexer.peek(1).token_type;
    if(t==DEFAULT){
        parse_default_case();
    }else if(t==RBRACE){
        ; // do nothing, fix later.
    }else{
        raise_error();
    }
    expect(RBRACE);
    return NULL; // placeholder
}

InstructionNode* parse_for_stmt(InstructionNode* stmt){
    // for_stmt -> FOR LPAREN...
    InstructionNode* assign1 = new InstructionNode();
    InstructionNode* assign2 = new InstructionNode();
    expect(FOR);
    expect(LPAREN);
    parse_assign_stmt(assign1);
    parse_condition();
    expect(SEMICOLON);
    parse_assign_stmt(assign2);
    expect(RPAREN);

    parse_body();
    return NULL; // placeholder
}

void parse_case_list(){
    // case_list -> case case_list | case
    parse_case();
    TokenType t = lexer.peek(1).token_type;
    if(t==CASE){
        parse_case_list();
    }else if(t==DEFAULT or t==RBRACE){
        ; // end of case list
    }else{
        raise_error();
    }
}

void parse_case(){
    expect(CASE);
    expect(NUM);
    expect(COLON);
    parse_body();
}

void parse_default_case(){
    expect(DEFAULT);
    expect(COLON);
    parse_body();
}

// inputs is a list of input values. Consume one at every input statement?
void parse_inputs(){
    parse_num_list();
}

void parse_num_list(){
    // num_list -> NUM | NUM num_list
    Token val = expect(NUM);
    add_input(val); // add Token's integer value to `inputs` vect

    TokenType t = lexer.peek(1).token_type;
    if(t==NUM){
        parse_num_list();
    }else if(t==END_OF_FILE){
        ; // end of case list
    }else{
        raise_error();
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
