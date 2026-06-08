
#include "builder.h"


string Builder::get_new_temp()
{
    return "t" + to_string(counter++);
}

void Builder::emit(string instruction)
{
    if (functions.empty())
    {
        cout << "Fatal IR error: tried to emit instruction outside of a function block\n";
        exit(1);
    }
    functions.back().instructions.push_back(instruction);
}

void Builder::start_function(string name)
{
    IRFunction new_function;
    new_function.name = name;
    functions.push_back(new_function);
    counter = 0;
}