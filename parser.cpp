
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

ASTNode* Parser::primary()
{
    if (check(TokenType::NUMBER))
        return CreateConstantNode(stod(advance().lexeme));

    if (check(TokenType::IDENTIFIER))
        return CreateVariableNode(advance().lexeme);

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
        char op = advance().lexeme[0];
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
        char op = advance().lexeme[0];
        ASTNode* right = term();
        expr = CreateOpNode(op, expr, right);
    }
    return expr;
}

ASTNode* Parser::assign()
{
    string varname = advance().lexeme;
    consume(TokenType::EQUAL, "Expected '=' after variable name");

    auto left = static_cast<ASTVariableNode*>(CreateVariableNode(varname));
    ASTNode* right = expression();

    consume(TokenType::SEMICOLON, "Expected ';' after statement");

    return CreateAssignmentNode(left, right);
}

ASTNode* Parser::declare()
{
    advance();
    if (check(TokenType::IDENTIFIER))
    {
        string varname = advance().lexeme;
        
        if (check(TokenType::EQUAL))
        {
            advance();
            ASTNode* right = expression();
            consume(TokenType::SEMICOLON, "Expected ';' after statement");
            
            return CreateDeclarationNode(varname, right);
        }

        if (check(TokenType::SEMICOLON))
        {
            advance();
            return CreateDeclarationNode(varname);
        }
    }
    if (check(TokenType::ENTRY_POINT))
    {
        advance();
        consume(TokenType::LEFT_PAREN, "Expected '(' for parameters");
        consume(TokenType::RIGHT_PAREN, "Expected ')' after parameters");
        consume(TokenType::LEFT_BRACE, "Expected '{' for function body");

        vector<ASTNode*> statements = this->parseBlock();
        ASTNode* mainFunction = CreateFunctionNode(statements);
        consume(TokenType::RIGHT_BRACE, "Expected '}' after function body");
        return mainFunction;
    }

    cout << "Parser Error: unexpected token " << peek().lexeme << '\n';
    exit(1);
}

ASTNode* Parser::printStatement()
{
    advance(); 
    consume(TokenType::LEFT_PAREN, "Expected '(' after function call name");
    ASTNode* expr = expression();
    consume(TokenType::RIGHT_PAREN, "Expected ')' to close function parameter");
    consume(TokenType::SEMICOLON, "Expected ';' after statement");

    return CreatePrintNode(expr);
}

vector<ASTNode*> Parser::parse() 
{
    vector<ASTNode*> program;
    
    while (!isAtEnd()) 
    {
        if (check(TokenType::BYTE))
            program.push_back(declare());
        else
        {
            cout << "Fatal error: unexpected token '" << peek().lexeme << "'\n";
            exit(1);
        }
    }

    return program;
}

vector<ASTNode*> Parser::parseBlock()
{
    vector<ASTNode*> block;
    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd())
    {
        if (check(TokenType::BYTE))
            block.push_back(declare());
        else if (check(TokenType::IDENTIFIER))
            block.push_back(assign());
        else if (check(TokenType::PRINT))
            block.push_back(printStatement());
        else {
            cout << "Fatal error: unexpected token '" << peek().lexeme << "'\n";
            exit(1);
        }
    }
    
    return block;
}