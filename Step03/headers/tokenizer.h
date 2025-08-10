#pragma once

#include <string>
#include <vector>
#include <optional>

enum class TokenType {
	exit,
	int_lit,
	semicolon,
	open_paren,
	close_paren,
	identifier,
	var,
	eq
};

struct Token {
	TokenType type;
	std::optional<std::string> value;
};

class Tokenizer {
	public:
		explicit Tokenizer(std::string src) : m_src(std::move(src)) {};
		std::vector<Token> tokenize();
	private:
		const std::string m_src;
		size_t m_index = 0;
		[[nodiscard]] std::optional<char> peek(int offset = 0) const;
		char consume();
};