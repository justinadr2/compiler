
#include "builder.h"


string Builder::get_new_temp()
{
    return "t" + to_string(counter++);
}


void Builder::emit(string instruction)
{
    instructions.push_back(instruction);
}