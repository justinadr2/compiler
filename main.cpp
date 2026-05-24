

/*
    Lexical analysis
        Linear text scanning
    Syntax analysis (parsing) 
    Semantic analysis
        type checking
        scope checking
        symbol table
    Intermediate representation
    Optimization
*/

/*
    No need to compile to object files, source code -> executable
    
    We need:
    Types
    Functions
    Local variables
    Loops
    Branching
    Pointers
    Standard template library for
        printing
*/

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include <cctype>
#include <vector>
#include <algorithm>

#include <intrin.h>

#include "ast.h"
#include "interpreter.h"
#include "lexer.h"
#include "parser.h"

using namespace std;

string ReadFile(string filename)
{
    ifstream file(filename);

    stringstream buffer;
    
    buffer << file.rdbuf();

    string code = buffer.str();

    file.close();

    return code;
}


int main()
{
    string filename = "code.bin";
    string code = ReadFile(filename);

    Lexer lexer(code);
    lexer.scan();
    
    Parser parser(lexer.tokens);
    parser.parseAndExecute();

    uint64_t end = __rdtsc();
}  