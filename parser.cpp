
#include <iostream>

#include "parser.h"

Parser::Parser(vector<Token> tokens) : tokens(tokens) 
{

}

bool Parser::isAtEnd() 
{ 
    return tokens[current].type == TokenType::END_OF_FILE; 
}

Token Parser::peek() 
{ 
    return tokens[current]; 
}

Token Parser::advance() 
{ 
    if (!isAtEnd()) 
        current++;

    return tokens[current - 1];
}

bool Parser::check(TokenType type) 
{ 
    if (isAtEnd()) 
        return false; 
    return peek().type == type; 
}
        
Token Parser::consume(TokenType type, string msg) 
{
    if (check(type)) 
        return advance();

    cout << "Parser Error: " << msg << " but found '" << peek().lexeme << "'\n";
    exit(1);
}

ASTNode* Parser::primary()
{
    if (check(TokenType::NUMBER))
        return CreateConstantNode(stod(advance().lexeme));

    if (check(TokenType::IDENTIFIER))
    {
        string name = advance().lexeme;
        return CreateVariableNode(name);
    }
    
    if (check(TokenType::LEFT_PAREN))
    {
        advance();
        ASTNode* expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expected ')' after expression");
        return expr;
    }

    cout << "Parser Error: Expected expression but found '" << peek().lexeme << "'\n";
    exit(1);    
}

ASTNode* Parser::term()
{
    ASTNode* expr = primary();
    while (check(TokenType::STAR) || check(TokenType::SLASH))
    {
        Token opToken = advance();
        char op = opToken.lexeme[0];
        ASTNode* right = primary();
        expr = CreateOpNode(op, expr, right);
    }
    return expr;
}   

ASTNode* Parser::expression()
{
    ASTNode* expr = term();
    while (check(TokenType::PLUS) || check(TokenType::MINUS))
    {
        Token opToken = advance();
        char op = opToken.lexeme[0];
        ASTNode* right = term();
        expr = CreateOpNode(op, expr, right);
    }
    return expr;
}

ASTNode* Parser::declaration()
{
    if (check(TokenType::BYTE) || check(TokenType::WORD) || check(TokenType::DWORD) || check(TokenType::QWORD)) 
    {
        advance();
        Token var = consume(TokenType::IDENTIFIER, "Expected variable name");
        consume(TokenType::EQUAL, "Expected '=' after variable name");
        
        auto left = static_cast<ASTVariableNode*>(CreateVariableNode(var.lexeme));
        ASTNode* right = expression();
        
        consume(TokenType::SEMICOLON, "Expected ';'");

        return CreateAssignmentNode(left, right);
    }

    if (check(TokenType::IDENTIFIER))
    {
        Token var = advance();
        consume(TokenType::EQUAL, "Expected '=' after variable name");

        auto left = static_cast<ASTVariableNode*>(CreateVariableNode(var.lexeme));
        ASTNode* right = expression();

        consume(TokenType::SEMICOLON, "Expected ';'");

        return CreateAssignmentNode(left, right);
    }
    
    if (check(TokenType::OUT)) 
    {
        advance();
        ASTNode* expr = expression();
        consume(TokenType::SEMICOLON, "Expected ';'");

        return new ASTPrintNode(expr);
    }
    
    cout << "Parser Error: Unexpected token " << tokens[current].lexeme << "\n";
    exit(1);
}

vector<ASTNode*> Parser::parse() 
{
    vector<ASTNode*> statements;
    
    while (!isAtEnd()) 
    {
        statements.push_back(declaration());
    }
    
    return statements;
}