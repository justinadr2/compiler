#pragma once

#include <string>
#include <vector>

using namespace std;

// IR Builder
class Builder
{
public:
    // temp_counter
    int counter = 0;
    vector<string> instructions;

    string get_new_temp();  

    void emit(string instruction);
};