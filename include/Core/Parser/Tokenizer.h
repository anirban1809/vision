#pragma once

#include <string>
#include <vector>
enum class TokenType {
    OpenTagStart,   // `<tag`
    CloseTagStart,  // `</tag`
    TagEnd,         // `>`
    SelfTagEnd,     // `/>`
    Identifier,     // tag names and attribute names
    Equals,         // `=`
    QuotedString,   // `"value"`
    TextContent,    // text between tags
    NewLine,        // NewLine character
    Space,          // empty space
    EndOfFile,
};

struct Token {
    TokenType type;
    std::string value;

    Token(TokenType type, const std::string& value)
        : type(type), value(value) {}
};

class Tokenizer {
   public:
    explicit Tokenizer(const std::string& filename);
    void Reset() { position = 0; }
    Token Next();
    Token CurrentToken();
    std::vector<Token> Tokenize();
    void Show();  // temporary function to view all the tokens;
    char Current();
    char Peek();          // peek the next character
    char PeekPrevious();  // peek the previous character
    void Move(unsigned int step);
    void ProcessString();
    void ProcessIdentifier();
    void ProcessTextContent();
    Token Last();

   private:
    unsigned int position = 0;  // current char position
    unsigned int token_position = 0;
    std::string source;
    std::vector<Token> tokens;
};