#include "parser.h"

/*
This file contains functions which are not parse_whatever functions.
Might need to combine this with parser.cc if problems keep arising... damn my lack of C++ file management understanding.
*/

//map<string, int> loctable;

// Return location in mem of given variable name
int location(string name){
    return loctable[name];
}
// Return location in mem of given token (overloaded function of `location`)
int location(Token t){
    return loctable[t.lexeme];
}

// Add new variable name to mem and iterate mem "current index" variable
void new_variable(string name){
    if(loctable.count(name) > 0){
        // do nothing if value already exists in location table
        // This is mainly for constant values since Bazzi specified to add constants to mem/location table as well.
        return;
    }else{
        int new_address = next_available;
        mem[new_address] = 0;
        loctable.emplace(name, new_address); // add mapping from variable name -> variable location (index in mem)

        next_available++;
    }
}

// Set value of `name`=`value` in mem[].
void set_value(string name, int value){
    mem[location(name)] = value;
}

// Given name of variable, assign its value as the next value in `inputs`.
void assign_next_input(string name){
    mem[location(name)] = inputs[next_input];
    next_input++;
    // inputs can be thought of as a stack.
}

// Push int value of Token `t` to `inputs` vector. (used in parsing `inputs` section)
void add_input(Token t){
    int val = stoi(t.lexeme);
    inputs.push_back(val);
}