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
            scope resolution
            function validation
            symbol table
        
        Intermediate representation
        
        Optimization
*/

/*
    No need to compile to object files, source code -> executable
    
    goals for now:
        allow declaration of unintialized variables
        custom functions with parameters

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

string TokenTypeToString(TokenType type)
{
    switch (type)
    {
        case TokenType::PLUS:        return "(PLUS)";
        case TokenType::MINUS:       return "(MINUS)";
        case TokenType::STAR:        return "(STAR)";
        case TokenType::SLASH:       return "(SLASH)";
        case TokenType::EQUAL:       return "(EQUAL)";
        case TokenType::SEMICOLON:   return "(SEMICOLON)";
        case TokenType::LEFT_PAREN:  return "(LEFT_PAREN)";
        case TokenType::RIGHT_PAREN: return "(RIGHT_PAREN)";
        case TokenType::IDENTIFIER:  return "(IDENTIFIER)";
        case TokenType::NUMBER:      return "(NUMBER)";
        case TokenType::BYTE:        return "(BYTE)";
        case TokenType::OUT:         return "(OUT)";
        case TokenType::END_OF_FILE: return "";
        default:                     return "UNKNOWN SYMBOL";
    }
}

int main()
{
    string code = ReadFile("code.bin");

    Lexer lexer(code);
    lexer.scan();
    
    Parser parser(lexer.tokens);
    vector<ASTNode*> program = parser.parse();

    ASTNode* mainFunction = program[0];
    
    mainFunction->evaluate();
    delete mainFunction;
}  
