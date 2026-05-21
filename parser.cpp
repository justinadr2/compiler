
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
    cout << "Parser Error: " << msg << " but found '" << peek().lexme << "'\n";
    exit(1);
}

ASTNode* Parser::primary()
{
    if (check(TokenType::NUMBER))
    {
        return CreateNumberNode(stod(advance().lexme));
    }
    if (check(TokenType::IDENTIFIER))
    {
        ASTNode* node = new ASTNode(ASTNodeType::NODE_IDENTIFIER);
        string name = advance().lexme;
        for(size_t i=0; i<63 && i<name.length(); ++i) node->data.name[i] = name[i];
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

    cout << "Parser Error: Expected expression but found '" << peek().lexme << "'\n";
    exit(1);
}

ASTNode* Parser::term()
{
    ASTNode* expr = primary();
    while (check(TokenType::STAR) || check(TokenType::SLASH))
    {
        Token opToken = advance();
        char op = opToken.lexme[0];
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
        char op = opToken.lexme[0];
        ASTNode* right = term();
        expr = CreateOpNode(op, expr, right);
    }
    return expr;
}

void Parser::parseAndExecute() 
{
    while (!isAtEnd()) 
    {
        if (check(TokenType::SET)) 
        {
            advance();
            Token var = consume(TokenType::IDENTIFIER, "Expected variable name");
            
            ASTNode* left = new ASTNode(ASTNodeType::NODE_IDENTIFIER);
            for (size_t i = 0; i < 63 && i < var.lexme.length(); i++) 
                left->data.name[i] = var.lexme[i];
            
            left->data.name[min(var.lexme.length(), (size_t)63)] = '\0';

            ASTNode* right = expression();
            consume(TokenType::SEMICOLON, "Expected ';'");

            ASTNode* stmt = new ASTNode(ASTNodeType::NODE_ASSIGNEMENT);
            stmt->left = left;
            stmt->right = right;
            
            Evaluate(stmt);
            delete stmt;
        }
        else if (check(TokenType::GET)) 
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
            cout << "Parser Error: Unexpected token " << tokens[current].lexme << "\n";
            exit(1);
        }
    }
}
