#ifndef __PARSER_H__
#define __PARSER_H__
/*
Alexander Ono
CSE340 - Bazzi
Project #3
11/24/2022

This file contains forward declarations for parser.cc and other.cc
*/

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <vector>
#include <string>
#include <set>
#include <vector>
#include "lexer.h"
#include "execute.h"
#include <map>

using namespace std;
extern map<string, int> loctable;


// maybe don't use this?
class Parser {
    public:
    private:
};

// Parsing Functions
void readAndPrintAllInput();
void parse_var_section();
void parse_id_list();
InstructionNode* parse_body();
InstructionNode* parse_stmt_list(); 
InstructionNode* parse_stmt();

InstructionNode* parse_assign_stmt(InstructionNode* stmt);
void parse_expr(InstructionNode* stmt);
int parse_primary();
ArithmeticOperatorType parse_op();

InstructionNode* parse_output_stmt(InstructionNode* stmt);
InstructionNode* parse_input_stmt(InstructionNode* stmt);
InstructionNode* parse_while_stmt(InstructionNode* stmt);
InstructionNode* parse_if_stmt(InstructionNode* stmt);
void parse_condition(InstructionNode* stmt);
ConditionalOperatorType parse_relop();
InstructionNode* parse_switch_stmt(InstructionNode* stmt);
InstructionNode* parse_for_stmt(InstructionNode* stmt);
InstructionNode* parse_case_list(InstructionNode* stmt);
InstructionNode* parse_case(InstructionNode* stmt);
InstructionNode* parse_default_case(InstructionNode* stmt);

void parse_inputs();
void parse_num_list();

// Non-Parsing Functions
void raise_error();
int location(string a);
int location(Token token);
void new_variable(string name);
void assign_next_input(string name);
void add_input(Token t);
void set_value(string name, int value);
InstructionNode* newNode();
InstructionNode* findTail(InstructionNode* node);

#endif