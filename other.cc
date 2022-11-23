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