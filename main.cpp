

/*
    Lexical analysis
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


#include "ast.h"
#include "interpreter.h"

char* filename;

using namespace std;

void TestEval()
{
    ASTNode* num3 = CreateNumberNode(3.0);
    ASTNode* num2 = CreateNumberNode(2.0);
    ASTNode* multiply = CreateOpNode('*', num3, num2);

    ASTNode* num5 = CreateNumberNode(5.0);
    ASTNode* root_add = CreateOpNode('+', num5, multiply);

    double result = Evaluate(root_add);

    printf("result: %f\n", result);
}

enum class TokenType
{
    PLUS, MINUS, STAR, SLASH, SEMICOLON, EQUAL,
    LEFT_PAREN, RIGHT_PAREN,
    IDENTIFIER, NUMBER,
    SET, GET,
    END_OF_FILE
};

struct Token
{
    TokenType type;
    string lexme;
};

class Lexer
{
public:
    string source;
    int current = 0;
    int start = 0;
    vector<Token> tokens;

    Lexer(string code) : source(code) {}
    
    bool isAtEnd()
    {
        return current >= source.length();
    }

    char advance()
    {
        return source[current++];
    }

    char peek()
    {
        if (isAtEnd())
            return '\0';
        return source[current];
    }

    void scan()
    {
        while (!isAtEnd())
        {
            start = current;
            char c = advance();

            if (isspace(c)) {
                continue; 
            }

            if (c == ';') 
                tokens.push_back({TokenType::SEMICOLON, ";"});
        
            else if (c == '+') 
                tokens.push_back({TokenType::PLUS, "+"});
        
            else if (c == '-') 
                tokens.push_back({TokenType::MINUS, "-"});
        
            else if (c == '*') 
                tokens.push_back({TokenType::STAR, "*"});

            else if (c == '/') 
                tokens.push_back({TokenType::SLASH, "/"});

            else if (c == '(')
                tokens.push_back({TokenType::LEFT_PAREN, "("});

            else if (c == ')')
                tokens.push_back({TokenType::RIGHT_PAREN, ")"});
            
            else if (isdigit(c)) 
            {
                while (isdigit(peek())) 
                    advance();
                
                string text = source.substr(start, current - start);
                tokens.push_back({TokenType::NUMBER, text});
            }

            else if (isalpha(c)) 
            {
                while (isalnum(peek())) advance();
                
                string text = source.substr(start, current - start);
                
                if (text == "set") {
                    tokens.push_back({TokenType::SET, text});
                } else if (text == "get") {
                    tokens.push_back({TokenType::GET, text});
                } else {
                    tokens.push_back({TokenType::IDENTIFIER, text});
                }
            }
        }
        
        tokens.push_back({TokenType::END_OF_FILE, ""});
    }
    
};

class Parser 
{
public:
    vector<Token> tokens;
    int current = 0;

    bool isAtEnd() 
    { 
        return tokens[current].type == TokenType::END_OF_FILE; 
    }

    Token peek() 
    { 
        return tokens[current]; 
    }

    Token advance() 
    { 
        if (!isAtEnd()) 
            current++; 
        return tokens[current - 1];
    }

    bool check(TokenType type) 
    { 
        if (isAtEnd()) 
            return false; 
        return peek().type == type; 
    }
    
    Token consume(TokenType type, string msg) 
    {
        if (check(type)) 
            return advance();
        cout << "Parser Error: " << msg << " but found '" << peek().lexme << "'\n";
        exit(1);
    }

    ASTNode* primary()
    {
        if (check(TokenType::NUMBER))
        {
            return CreateNumberNode(stod(advance().lexme));
        }
        if (check(TokenType::IDENTIFIER))
        {
            ASTNode* node = CreateNode(NodeType::NODE_IDENTIFIER);
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

    ASTNode* term()
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

    ASTNode* expression()
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

    Parser(vector<Token> tokens) : tokens(tokens) {}

    void parseAndExecute() 
    {
        while (!isAtEnd()) 
        {
            if (check(TokenType::SET)) 
            {
                advance();
                Token var = consume(TokenType::IDENTIFIER, "Expected variable name");
                
                ASTNode* left = CreateNode(NodeType::NODE_IDENTIFIER);
                for (size_t i=0; i<63 && i<var.lexme.length(); ++i) 
                    left->data.name[i] = var.lexme[i];
                
                left->data.name[min(var.lexme.length(), (size_t)63)] = '\0';

                ASTNode* right = expression();
                consume(TokenType::SEMICOLON, "Expected ';'");

                ASTNode* stmt = CreateNode(NodeType::NODE_ASSIGNEMENT);
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
};

string TokenTypeToString(TokenType type) 
{
    switch (type) 
    {
        case TokenType::SET: return "SET";
        case TokenType::GET: return "GET";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::STAR: return "STAR";
        case TokenType::SLASH: return "SLASH";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::EQUAL: return "EQUAL";
        case TokenType::END_OF_FILE: return "EOF";
        case TokenType::LEFT_PAREN: return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
        default: return "UNKNOWN";
    }
}

void Read(string& code)
{
    ifstream file(filename);

    stringstream buffer;
    
    buffer << file.rdbuf();

    code = buffer.str();

    file.close();
}

int main()
{
    filename = "code.bin";
    string code;

    Read(code);

    Lexer lexer(code);
    lexer.scan();   

    Parser parser(lexer.tokens);
    parser.parseAndExecute();

}



