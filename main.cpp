/*
    overview: 
        Read file contents to memory

        Lexical analysis
            tokens
            lexer

        Syntax analysis  
            parser        

        Semantic analysis
            type checking
            scope checking
            symbol table
        
        Intermediate representation
        
        Optimization
*/

/*
    No need to compile to object files, source code -> executable
    
    goals for now:
        allow declaratin of unintialized variables
        classes for each specific AST node
        functions and entry point

*/

/*
    types of AST nodes:
        Leaf nodes
            Constant nodes
            Identifier nodes

        Expression nodes
            Binary op (x + y)
            Unary op (!x)
            Assignment nodes (x = 10)
    
        Statement nodes
            If-statement nodes (condition, then_block (block node), else_block (block node))
            Loop nodes (condition, body (block node))
            Block nodes (scopes)

        Declaration nodes
            Function definition nodes (type, name, params, body (block node))
            Variable declaration nodes (type, name)
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
#include "lexer.h"
#include "parser.h"

using namespace std;

unordered_map<string, double> symbolTable;

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
    string code = ReadFile("code.bin");

    Lexer lexer(code);
    lexer.scan();

    Parser parser(lexer.tokens);
    vector<ASTNode*> program = parser.parse();

    for (ASTNode* statement : program)
    {
        statement->evaluate();
        delete  statement;
    }
}  
