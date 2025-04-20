#include "Core/Element.h"
#include "Core/Parser/Parser.h"
#include "Core/Parser/Tokenizer.h"
#include <memory>
#include <stdexcept>

std::shared_ptr<Element> Parser::Parse() { return ParseElement(); }

std::shared_ptr<Element> Parser::ParseElement() {
    Expect(TokenType::OpenTagStart, "Expected Opening tag");
    std::string tag_name = m_CurrentToken.value;
    Advance();  // consume tag name

    std::shared_ptr<Element> element = std::make_shared<Element>(tag_name);
    ParseAttributes(element);

    if (Match(TokenType::SelfTagEnd)) {
    }

    if (Match(TokenType::TagEnd)) {
        ParseChildren(element);

        Expect(TokenType::CloseTagStart, "Expected closing tag");
        std::string close_name = m_CurrentToken.value;
        Advance();
        if (close_name != tag_name)
            throw std::runtime_error("Tag mismatch: " + tag_name + " vs " +
                                     close_name);
        Expect(TokenType::TagEnd, "Expected '>' after closing tag");
    }

    return element;
}

void Parser::ParseAttributes(std::shared_ptr<Element>& element) {
    while (m_CurrentToken.type == TokenType::Identifier) {
        std::string key = m_CurrentToken.value;

        // consume the key
        Advance();

        Expect(TokenType::Equals, "Expected \"=\" after attribute name");

        if (m_CurrentToken.type == TokenType::QuotedString) {
            std::string value = m_CurrentToken.value;
            element->attributes[key] = value;
            Advance();
        } else {
            throw std::runtime_error("Expected quoted attribute value");
        }
    }
}

void Parser::ParseChildren(std::shared_ptr<Element>& parent) {
    while (true) {
        if (m_CurrentToken.type == TokenType::CloseTagStart) {
            break;
        }

        if (m_CurrentToken.type == TokenType::OpenTagStart) {
            parent->AddChild(ParseElement());
        }

        else if (m_CurrentToken.type == TokenType::TextContent) {
            parent->innerText += m_CurrentToken.value;
            Advance();
        } else {
            break;
        }
    }
}

void Parser::Advance() { m_CurrentToken = m_Tokenizer.Next(); }

bool Parser::Match(TokenType type) {
    if (m_CurrentToken.type == type) {
        Advance();
        return true;
    }
    return false;
}

void Parser::Expect(TokenType type, const std::string& errorMessage) {
    if (!Match(type)) throw std::runtime_error(errorMessage);
}