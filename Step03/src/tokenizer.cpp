#include <iostream>

#include "../headers/tokenizer.h"

std::vector<Token> Tokenizer::tokenize() {
	std::vector<Token> tokens;
	std::string buffer;

	while(peek().has_value()) {
		if (std::isalpha(peek().value())) {
			buffer.push_back(consume());
			while (peek().has_value() && std::isalnum(peek().value())) {
				buffer.push_back(consume());
			}
			if (buffer == "exit") {
				tokens.push_back({ .type = TokenType::exit});
				buffer.clear();
				continue;
			} else if (buffer == "var") {
				tokens.push_back({ .type = TokenType::var});
				buffer.clear();
				continue;
			// If the text is not a keyword, treat it as an identifier (variable name)
			} else {
				tokens.push_back({ .type = TokenType::identifier, .value = buffer});
				buffer.clear();
				continue;
			}
		} else if (std::isdigit(peek().value())) {
			buffer.push_back(consume());
			while (peek().has_value() && std::isdigit(peek().value())) {
				buffer.push_back(consume());
			}
			tokens.push_back({.type = TokenType::int_lit, .value = buffer});
			buffer.clear();
			continue;
		} else if (peek().value() == '(') {
			consume();
			tokens.push_back({.type = TokenType::open_paren});
			continue;
		} else if (peek().value() == ')') {
			consume();
			tokens.push_back({.type = TokenType::close_paren});
			continue;
		} else if (peek().value() == ';') {
			consume();
			tokens.push_back({.type = TokenType::semicolon});
			continue;
		} else if (peek().value() == '=') {
			consume();
			tokens.push_back({.type = TokenType::eq});
			continue;
		} else if (std::isspace(peek().value())) {
			consume();
			continue;
		} else {
			std::cerr << "You messed up! (2)" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	m_index = 0;

	return tokens;
}

[[nodiscard]] std::optional<char> Tokenizer::peek(int offset) const {
	if (m_index + offset >= m_src.length()) {
		return {};
	} else {
		return m_src.at(m_index + offset);
	}
};

char Tokenizer::consume() {
	return m_src.at(m_index++);
};