#pragma once

#include "Core/Element.h"
#include "Core/Parser/Tokenizer.h"
#include <memory>
class Parser {
   public:
    Parser(Tokenizer& tokenizer)
        : m_Tokenizer(tokenizer), m_CurrentToken(tokenizer.CurrentToken()) {}

    std::shared_ptr<Element> Parse();

   private:
    Tokenizer& m_Tokenizer;
    Token m_CurrentToken;

    void Advance();
    bool Match(TokenType type);
    void Expect(TokenType type, const std::string& err);

    std::shared_ptr<Element> ParseElement();
    void ParseAttributes(std::shared_ptr<Element>& element);
    void ParseChildren(std::shared_ptr<Element>& parent);
};