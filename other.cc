#include "parser.h"

/*
This file contains functions which are not parse_whatever functions.
Might need to combine this with parser.cc if problems keep arising... damn my lack of C++ file management understanding.
*/

map<string, int> loctable;

// Return location in mem of given variable name
int location(string name){
    return loctable[name];
}

// Add new variable name to mem and iterate mem "current index" variable
void new_variable(string name){
    int new_address = next_available;
    mem[new_address] = 0;
    //loctable.insert({name, new_address}); // mapping from variable name -> variable location (index in mem)
    loctable.emplace(name, new_address); // add mapping from variable name -> variable location (index in mem)
    //loctable.clear();

    next_available++;
}

// Given name of variable, assign its value as the next input value.
void assign_next_input(string name){
    mem[location(name)] = inputs[next_input];
    next_input++;
    // inputs can be thought of as a stack.
}

// Push int value of `t` to `inputs` vector.
void add_input(Token t){
    int val = stoi(t.lexeme);
    inputs.push_back(val);
}