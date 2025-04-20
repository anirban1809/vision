#include "Core/Parser/Tokenizer.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

Tokenizer::Tokenizer(const std::string& filename) {
    std::ifstream file(filename);

    if (!file) {
        std::cerr << "Failed to open source file. Exiting" << std::endl;
        exit(1);
    }

    source.assign((std::istreambuf_iterator<char>(file)),
                  std::istreambuf_iterator<char>());
}

Token Tokenizer::CurrentToken() { return tokens[token_position]; }
Token Tokenizer::Next() {
    token_position++;
    return tokens[token_position];
}

char Tokenizer::Current() { return source.at(position); }

char Tokenizer::Peek() { return source.at(position + 1); }
char Tokenizer::PeekPrevious() { return source.at(position - 1); }
void Tokenizer::Move(unsigned int step) { position += step; }

void Tokenizer::ProcessString() {
    std::string value;

    while (Current() != '"') {
        value.push_back(Current());
        Move(1);
    }

    tokens.push_back(Token(TokenType::QuotedString, value));
}

void Tokenizer::ProcessIdentifier() {
    std::string value;

    while (Current() != ' ' && Current() != '=' && Current() != '>' &&
           Current() != '\n') {
        value.push_back(Current());
        Move(1);
    }

    tokens.push_back(Token(TokenType::Identifier, value));
}

void Tokenizer::ProcessTextContent() {
    std::string value;

    while (Current() != '<' && position < source.length() - 1) {
        if (Current() != '\n') {
            value.push_back(Current());
        }
        Move(1);
    }

    // trim the leading and trailing whitspaces
    value = std::regex_replace(value, std::regex("^ +| +$|( ) +"), "$1");
    if (value.length() > 0) {
        tokens.push_back(Token(TokenType::TextContent, value));
    }
}

Token Tokenizer::Last() { return tokens[tokens.size() - 1]; }

std::vector<Token> Tokenizer::Tokenize() {
    while (position < source.length()) {
        char c = source.at(position);
        switch (c) {
            case '<':
                if (Peek() == '/') {
                    Move(2);
                    tokens.push_back(Token(TokenType::CloseTagStart, "</"));
                } else {
                    Move(1);
                    tokens.push_back(Token(TokenType::OpenTagStart, "<"));
                }
                break;

            case '>':
                Move(1);  // consume the >
                tokens.push_back(Token(TokenType::TagEnd, ">"));
                ProcessTextContent();
                break;
            case '/':
                if (Peek() == '>') {
                    Move(2);
                    tokens.push_back(Token(TokenType::SelfTagEnd, "/>"));
                }
                break;
            case '=':
                Move(1);
                tokens.push_back(Token(TokenType::Equals, "="));
                break;

            case '"':
                Move(1);  // consume start "
                ProcessString();
                Move(1);  // consume end "
                break;

            case ' ':
                Move(1);
                break;

            case '\n':
                Move(1);
                break;

            default:
                ProcessIdentifier();
        }
    }

    return tokens;
}

void Tokenizer::Show() {
    for (auto& token : tokens) {
        std::cout << "Type: " << static_cast<int>(token.type)
                  << " Length: " << token.value.length()
                  << " Token: " << token.value << std::endl;
    }
}