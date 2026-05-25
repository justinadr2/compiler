#pragma once

#include <string>
#include <vector>

using namespace std;

enum class TokenType
{
    PLUS, MINUS, STAR, SLASH, SEMICOLON, EQUAL,
    LEFT_PAREN, RIGHT_PAREN,
    IDENTIFIER, NUMBER,

    BYTE, WORD, DWORD, QWORD,   

    OUT,
    END_OF_FILE
};

struct Token
{
    TokenType type;
    string lexeme;
};

class Lexer
{
public:
    vector<Token> tokens;
    string source;
    
    int current = 0;
    int start;

    Lexer(string code);

    bool isAtEnd();

    char advance();

    char peek();

    void scan();
};