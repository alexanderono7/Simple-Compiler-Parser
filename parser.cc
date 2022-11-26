#include "parser.h"
/*
Alexander Ono
CSE340 - Bazzi
Project #3
11/24/2022

This file mainly contains literal parsing functions.
*/

LexicalAnalyzer lexer;  // hopefully this won't cause issues, but I might need to combine parser.cc and other.cc into 1 file.
map<string, int> loctable;



Token expect(TokenType expected_type){
    Token t = lexer.GetToken();
    if(t.token_type != expected_type)
        raise_error();
    return t;
} 

// Equivalent to parse_program?
struct InstructionNode * parse_generate_intermediate_representation()
{
    //cout << "program executed!\n"; // remove later
    // program -> var_section body inputs
    InstructionNode* head;

    //raise_error(); // for testing/debugging
    parse_var_section();
    head = parse_body();
    parse_inputs();

    //cout << "program finished.\n"; // remove later
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

InstructionNode* parse_body(){
    // body -> LBRACE stmt_list RBRACE
    InstructionNode* instList;

    expect(LBRACE);
    instList = parse_stmt_list();
    expect(RBRACE);
    
    return instList;
}

InstructionNode* parse_stmt_list(){
    // stmt_list -> stmt stmt_list | stmt
    InstructionNode* inst; // instruction for one statement
    InstructionNode* instList; // instruction for statement list
    inst = parse_stmt();


    TokenType t = lexer.peek(1).token_type;
    // FIRST(stmt) = {ID, "output", "input", WHILE, IF, SWITCH, FOR}
    set<TokenType> stmt_fs({ID, OUTPUT, INPUT, WHILE, IF, SWITCH, FOR});  // first set of stmt
    if(stmt_fs.count(t)){ // check if t.token_type is in FIRST(stmt) to see if stmt_list continues
        instList = parse_stmt_list(); 
        findTail(inst)->next = instList; // append instList to the end of `inst`
    }else if(t==RBRACE){ // is this really accurate?
        // end of statement list
        ;
    }else{
        raise_error();
    }
    return inst; // placeholder
}

InstructionNode* parse_stmt(){
    // I believe new instruction nodes should be instanciated here.
    //InstructionNode* stmt = new InstructionNode();
    InstructionNode* stmt = newNode();

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
    stmt->type = ASSIGN;
    // assign_stmt -> ID EQUAL primary SEMICOLON | ID EQUAL expr SEMICOLON
    Token lhs = expect(ID);
    expect(EQUAL);
    
    // Get reference to LHS variable.
    stmt->assign_inst.left_hand_side_index = location(lhs.lexeme);

    TokenType t = lexer.peek(1).token_type;
    TokenType s = lexer.peek(2).token_type;
    // check if t is in FIRST(primary) or FIRST(expr)
    // FIRST(primary) = ID | NUM //only has 1 token
    // FIRST(expr) = ID | NUM 
    // SECOND(expr) = op
    if(t==ID or t==NUM){
        if(s==SEMICOLON){
            // No expression on RHS, just 1 integer/variable (e.g. x = 1;)
            stmt->assign_inst.opernd1_index = parse_primary(); // get reference to the lone primary on the RHS.
            stmt->assign_inst.op = OPERATOR_NONE;
        }else if(s==PLUS or s==MINUS or s==MULT or s==DIV){
            // There's an expression on RHS (e.g. x = 1+2;)
            parse_expr(stmt); // adds appropriate expression field members to `stmt`
        }else{
            raise_error();
        }
    }else{
        raise_error();
    }

    expect(SEMICOLON);
    return NULL; // placeholder
}

void parse_expr(InstructionNode* stmt){
    // expr -> primary op primary
    stmt->assign_inst.opernd1_index = parse_primary();
    stmt->assign_inst.op = parse_op();
    stmt->assign_inst.opernd2_index = parse_primary();
    /*
    Side note: as defined in the grammar, `expr` should only ever be used inside of an assignment statement, so it 
    should be fine to assume the `stmt` argument is always an assignment statement.
    */
}

// Returns the mem index of the primary it parsed
int parse_primary(){
    // primary -> ID | NUM
    Token t = lexer.peek(1);
    Token token;

    if(t.token_type == ID){
        token = expect(ID);
        new_variable(token.lexeme);
    }else if(t.token_type == NUM){
        token = expect(NUM);
        new_variable(token.lexeme);
        set_value(token.lexeme, stoi(token.lexeme)); // constants/literals should just have their lexeme as their integer value.
    }else{
        raise_error();
    }

    return location(token);
}

ArithmeticOperatorType parse_op(){
    // op -> PLUS | MINUS | MULT | DIV
    TokenType t = lexer.peek(1).token_type;
    Token token;
    switch(t){
        case PLUS:
            expect(PLUS);
            return OPERATOR_PLUS;
            break;
        case MINUS:
            expect(MINUS);
            return OPERATOR_MINUS;
            break;
        case MULT:
            expect(MULT);
            return OPERATOR_MULT;
            break;
        case DIV:
            expect(DIV);
            return OPERATOR_DIV;
            break;
        default:
            raise_error();
            break;
    }
    raise_error();
    return OPERATOR_NONE; // this should never be returned.
}

InstructionNode* parse_output_stmt(InstructionNode* stmt){
    string output_var; // name of variable to be outputted
    stmt->type = OUT;
    // output_stmt = OUTPUT ID SEMICOLON
    expect(OUTPUT);

    output_var = expect(ID).lexeme;
    stmt->output_inst.var_index = location(output_var);

    expect(SEMICOLON);
    return NULL; // placeholder
}

InstructionNode* parse_input_stmt(InstructionNode* stmt){
    // input_stmt = INPUT ID SEMICOLON
    Token t;
    stmt->type = IN;
    expect(INPUT);
    t = expect(ID);
    expect(SEMICOLON);

    stmt->input_inst.var_index = location(t);
    return stmt; // placeholder
}

InstructionNode* parse_while_stmt(InstructionNode* inst){
    // while_stmt -> WHILE condition body
    inst->type = CJMP;
    expect(WHILE);

    parse_condition(inst);
    inst->next = parse_body();

    // Instanciate new JMP instruction node.
    InstructionNode* jmp = newNode();
    jmp->type = JMP;
    jmp->jmp_inst.target = inst;
    findTail(inst->next)->next = jmp; // append JMP node to body of WHILE loop
    // Instanciate new NOOP node.
    InstructionNode* nope = newNode();
    nope->type = NOOP;
    findTail(jmp)->next = nope; // append NOOP node to list of instructions after JMP node (?)
    inst->cjmp_inst.target = nope; // CJMP node's target is NOOP node

    return inst; // placeholder
}

InstructionNode* parse_if_stmt(InstructionNode* inst){
    // if_stmt -> IF condition body
    inst->type = CJMP;
    expect(IF);

    parse_condition(inst); // assigns conditional fields for `stmt`
    inst->next = parse_body();

    // Instanciate new NOOP instruction node.
    InstructionNode* nope = newNode();
    nope->type = NOOP;
    findTail(inst->next)->next = nope; // append NOOP node to body of if statement
    inst->cjmp_inst.target = nope; // CJMP target is NOOP node.

    return inst; // return head of the if statement
}

void parse_condition(InstructionNode* stmt){
    // condition -> primary relop primary
    stmt->type = CJMP;
    stmt->cjmp_inst.opernd1_index = parse_primary();
    stmt->cjmp_inst.condition_op = parse_relop();
    stmt->cjmp_inst.opernd2_index = parse_primary();
}

ConditionalOperatorType parse_relop(){
    // relop -> GREATER | LESS | NOTEQUAL
    TokenType t = lexer.peek(1).token_type;
    switch(t){
        case GREATER:
            expect(GREATER);
            return CONDITION_GREATER;
            break;
        case LESS:
            expect(LESS);
            return CONDITION_LESS;
            break;
        case NOTEQUAL:
            expect(NOTEQUAL);
            return CONDITION_NOTEQUAL;
            break;
        default:
            raise_error();
            break;
    }

    raise_error();
    return CONDITION_NOTEQUAL; // program should never reach this point.
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

InstructionNode* parse_for_stmt(InstructionNode* inst){
    // for_stmt -> FOR LPAREN...
    InstructionNode* assign1 = newNode(); // head of for loop
    InstructionNode* assign2 = newNode();
    InstructionNode* cjmp = newNode();
    InstructionNode* jmp = newNode();
    InstructionNode* nope = newNode();

    expect(FOR);
    expect(LPAREN);

    // assign1 node
    parse_assign_stmt(assign1);
    assign1->next = cjmp;

    // cjmp node
    parse_condition(cjmp);
    cjmp->cjmp_inst.target = nope;

    expect(SEMICOLON);

    // assign2 node
    parse_assign_stmt(assign2);
    assign2->next = jmp;

    // jmp node
    jmp->type = JMP;
    jmp->next = nope;
    jmp->jmp_inst.target = cjmp;

    expect(RPAREN);

    // body of for loop
    cjmp->next = parse_body();
    findTail(cjmp)->next = assign2;

    // noop node
    nope->type = NOOP;

    return assign1; // return head of for loop
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
