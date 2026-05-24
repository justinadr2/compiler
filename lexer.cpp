#include "lexer.h"
Lexer::Lexer(string code) : source(code) 
{

}
    
bool Lexer::isAtEnd()
{
    return current >= source.length();
}

char Lexer::advance()
{
    return source[current++];
}

char Lexer::peek()
{
    if (isAtEnd())
        return '\0';
    return source[current];
}

void Lexer::scan()
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

        else if (isalpha(c) || c == '_') 
        {
            while (isalnum(peek()) || peek() == '_') 
                advance();
            
            string text = source.substr(start, current - start);

            if (text == "set") 
                tokens.push_back({TokenType::SET, text});
            else if (text == "out") 
                tokens.push_back({TokenType::OUT, text});
            else 
                tokens.push_back({TokenType::IDENTIFIER, text});
        }
    }
    
    tokens.push_back({TokenType::END_OF_FILE, ""});
}