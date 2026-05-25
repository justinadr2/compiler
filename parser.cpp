
#include <iostream>

#include "parser.h"
#include "interpreter.h"

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
    {
        return CreateNumberNode(stod(advance().lexeme));
    }
    if (check(TokenType::IDENTIFIER))
    {
        string name = advance().lexeme;
        ASTNode* node = CreateIdentifierNode(name);
        node->data.name[min(name.length(), (size_t)63)] = '\0';

        return node;
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

void Parser::parseAndExecute() 
{
    while (!isAtEnd()) 
    {
        if (check(TokenType::BYTE)) 
        {
            advance();
            Token var = consume(TokenType::IDENTIFIER, "Expected variable name");

            consume(TokenType::EQUAL, "Expected '=' after variable name");
            
            ASTNode* left = CreateIdentifierNode(var.lexeme);
            left->data.name[min(var.lexeme.length(), (size_t)31)] = '\0';

            ASTNode* right = expression();
            consume(TokenType::SEMICOLON, "Expected ';'");

            ASTNode* statement = CreateAssignmentNode(left, right);
    
            Evaluate(statement);
            delete statement;
        }
        else if (check(TokenType::OUT)) 
        {
            advance();
            ASTNode* expr = expression();
            consume(TokenType::SEMICOLON, "Expected ';'");

            double val = Evaluate(expr);
            cout << "Out: " << val << "\n";
            delete expr;
        }
        else 
        {
            cout << "Parser Error: Unexpected token " << tokens[current].lexeme << "\n";
            exit(1);
        }
    }
}
