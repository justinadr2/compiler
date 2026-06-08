#pragma once

#include <string>
#include <vector>
#include <iostream>

using namespace std;

struct IRFunction
{
    string name;
    vector<string> instructions;
};

// IR Builder
class Builder
{
public:
    // temp_counter
    int counter = 0;
    vector<IRFunction> functions;

    IRFunction* current_function = nullptr;
    string get_new_temp();  
    void emit(string instruction);
    void start_function(string name);
};