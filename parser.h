#pragma once

#include <string>
#include <vector>

#include "lexer.h"
#include "ast.h"

class Parser 
{
public:
    vector<Token> tokens;
    int current = 0;

    Parser(vector<Token> tokens);

    Token peek();
    bool isAtEnd();
    Token advance();
    bool check(TokenType type);
    Token consume(TokenType type, string msg);

    ASTNode* primary();
    ASTNode* term();
    ASTNode* expression();

    ASTNode* declare();
    
    vector<ASTNode*> parse();
    vector<ASTNode*> parseBlock();
    
};