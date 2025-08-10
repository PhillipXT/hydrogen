#pragma once

#include <variant>

#include "tokenizer.h"

struct NodeExpressionIntegerLiteral {
	Token int_lit;
};

struct NodeExpressionIdentifier {
	Token identifier;
};

struct NodeExpression {
	std::variant<NodeExpressionIntegerLiteral, NodeExpressionIdentifier> var;
};

struct NodeStatementExit {
	NodeExpression expr;
};

struct NodeStatementVar {
	Token ident;
	NodeExpression expr;
};

struct NodeStatement {
	std::variant<NodeStatementExit, NodeStatementVar> var;
};

struct NodeProgram {
	std::vector<NodeStatement> statements;
};

class Parser {
	public:
		explicit Parser(std::vector<Token> tokens) : m_tokens(std::move(tokens)) {};
		std::optional<NodeExpression> parse_expr();
		std::optional<NodeStatement> parse_stmt();
		std::optional<NodeProgram> parse_program();
	private:
		const std::vector<Token> m_tokens;
		size_t m_index = 0;
		[[nodiscard]] std::optional<Token> peek(int offset = 0) const;
		Token consume();
};