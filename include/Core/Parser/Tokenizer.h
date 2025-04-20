#pragma once

#include <string>
#include <vector>
enum class TokenType {
    OpenTagStart,   // `<tag`
    CloseTagStart,  // `</tag`
    TagEnd,         // `>` or `/>`
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
    Token Next();
    void Tokenize();
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
    unsigned int position = 0;  // current position
    std::string source;
    std::vector<Token> tokens;
};