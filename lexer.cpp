#include "lexer.h"
Lexer::Lexer(string source) : source(source) 
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

        if (isspace(c))
            continue; 
        
        else if (isalpha(c) || c == '_') 
        {
            while (isalnum(peek()) || peek() == '_') 
                advance();
            
            string word = source.substr(start, current - start);

            if (word == "byte")
                tokens.push_back({TokenType::BYTE, word});
            else if (word == "out")
                tokens.push_back({TokenType::OUT, word});
            else if (word == "main")
                tokens.push_back({TokenType::ENTRY_POINT, word});
            else
                tokens.push_back({TokenType::IDENTIFIER, word});
        }

        else if (isdigit(c)) 
        {
            while (isdigit(peek())) 
                advance();
            
            string number = source.substr(start, current - start);
            tokens.push_back({TokenType::NUMBER, number});
        }

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
        else if (c == '=')
            tokens.push_back({TokenType::EQUAL, "="});
        else if (c == ';') 
            tokens.push_back({TokenType::SEMICOLON, ";"});   
        else if (c == '{') 
            tokens.push_back({TokenType::LEFT_BRACE, "{"});   
        else if (c == '}') 
            tokens.push_back({TokenType::RIGHT_BRACE, "}"});   
        
    }
    
    tokens.push_back({TokenType::END_OF_FILE, ""});
}