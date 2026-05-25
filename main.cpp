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
        define the keywords and syntax im gonna be using
        classes for each specific AST node
        functions and entry point
        local variabless

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

namespace dbg {
    string TokenTypeToString(TokenType type)
    {
        switch (type)
        {
            case TokenType::PLUS:        return "PLUS";
            case TokenType::MINUS:       return "MIN";
            case TokenType::STAR:        return "STAR";
            case TokenType::SLASH:       return "SLASH";
            case TokenType::SEMICOLON:   return "SEMICOLON";
            case TokenType::EQUAL:       return "EQUAL";
            case TokenType::IDENTIFIER:  return "IDENTIFIER";
            case TokenType::NUMBER:      return "NUMBER";
            case TokenType::LEFT_PAREN:  return "LEFT_PAREN";
            case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
            case TokenType::BYTE:        return "BYTE";
            case TokenType::WORD:        return "WORD";
            case TokenType::DWORD:       return "DWORD";
            case TokenType::QWORD:       return "QWORD";
            case TokenType::OUT:         return "OUT";
            case TokenType::END_OF_FILE: return "(END_OF_FILE)";
            default:                     return "UNKNOWN SYMBOL";
        }
    }
}

int main()
{
    string code = ReadFile("code.bin");

    Lexer lexer(code);
    lexer.scan();
    
    for (const Token& token : lexer.tokens) 
        cout << dbg::TokenTypeToString(token.type) << ": " << token.lexeme << '\n';

    cout << "current: " << lexer.current << '\n';

    Parser parser(lexer.tokens);
    parser.parseAndExecute();
}  
