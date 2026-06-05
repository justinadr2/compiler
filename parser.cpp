
#include <iostream>

#include "parser.h"

Parser::Parser(vector<Token> tokens) : tokens(tokens) 
{

}

Token Parser::peek() 
{ 
    return tokens[current]; 
}

bool Parser::isAtEnd() 
{ 
    return peek().type == TokenType::END_OF_FILE; 
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

Node* Parser::primary()
{
    if (check(TokenType::NUMBER))
        return CreateConstantNode(stod(advance().lexeme));

    if (check(TokenType::IDENTIFIER))
        return CreateGetVariableNode(advance().lexeme);

    if (check(TokenType::LEFT_PAREN))
    {
        advance();
        Node* expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expected ')' after expression");
        return expr;
    }

    cout << "Parser Error: Expected expression but found '" << peek().lexeme << "'\n";
    exit(1);    
}

Node* Parser::term()
{
    Node* expr = primary();
    while (check(TokenType::STAR) || check(TokenType::SLASH))
    {
        char op = advance().lexeme[0];
        Node* right = primary();
        expr = CreateBinaryOpNode(op, expr, right);
    }
    return expr;
}   

Node* Parser::expression()
{
    Node* expr = term();
    while (check(TokenType::PLUS) || check(TokenType::MINUS))
    {
        char op = advance().lexeme[0];
        Node* right = term();
        expr = CreateBinaryOpNode(op, expr, right);
    }
    return expr;
}

Node* Parser::assign()
{
    string varname = advance().lexeme;
    consume(TokenType::EQUAL, "Expected '=' after variable name");

    auto left = static_cast<GetVariable*>(CreateGetVariableNode(varname));
    Node* right = expression();

    consume(TokenType::SEMICOLON, "Expected ';' after statement");

    return CreateAssignmentNode(left, right);
}

Node* Parser::declare()
{
    advance();
    if (check(TokenType::IDENTIFIER))
    {
        string varname = advance().lexeme;
        
        if (check(TokenType::EQUAL))
        {
            advance();
            Node* right = expression();
            consume(TokenType::SEMICOLON, "Expected ';' after statement");
            
            return CreateDeclareVariableNode(varname, right);
        }

        if (check(TokenType::SEMICOLON))
        {
            advance();
            return CreateDeclareVariableNode(varname);
        }
    }

    cout << "Parser Error: unexpected token '" << peek().lexeme << "'\n";
    exit(1);
}

vector<Node*> Parser::parse() 
{
    vector<Node*> program;
    
    while (!isAtEnd()) 
    {
        if (check(TokenType::BYTE))
            program.push_back(declare());
        
        else if (check(TokenType::IDENTIFIER))
            program.push_back(assign());

        else
        {
            cout << "Fatal error: unexpected token '" << peek().lexeme << "'\n";
            exit(1);
        }
    }

    return program;
}