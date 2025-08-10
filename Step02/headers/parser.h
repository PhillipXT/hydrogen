#pragma once

#include "tokenizer.h"

struct NodeExpr {
	Token int_lit;
};

struct NodeExit {
	NodeExpr expr;
};

class Parser {
	public:
		explicit Parser(std::vector<Token> tokens) : m_tokens(std::move(tokens)) {};
		std::optional<NodeExpr> parse_expr();
		std::optional<NodeExit> parse();
	private:
		const std::vector<Token> m_tokens;
		size_t m_index = 0;
		[[nodiscard]] std::optional<Token> peek(int offset = 0) const;
		Token consume();
};