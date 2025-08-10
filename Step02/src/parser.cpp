#include <iostream>

#include "../headers/parser.h"

std::optional<NodeExpr> Parser::parse_expr() {
	if (peek().has_value() && peek().value().type == TokenType::int_lit) {
		return NodeExpr { .int_lit = consume() };
	} else {
		return {};
	}
};

std::optional<NodeExit> Parser::parse() {
	std::optional<NodeExit> exit_node;
	while (peek().has_value()) {
		if (peek().value().type == TokenType::exit) {
			consume();
			if (auto node_expr = parse_expr()) {
				exit_node = NodeExit { .expr = node_expr.value() };
			} else {
				std::cerr << "Invalid expression (1)" << std::endl;
				exit(EXIT_FAILURE);
			}
			if (peek().has_value() && peek().value().type == TokenType::semicolon) {
				consume();
			} else {
				std::cerr << "Invalid expression (2)" << std::endl;
				exit(EXIT_FAILURE);
			}
		}
	}
	m_index = 0;
	return exit_node;
};

[[nodiscard]] std::optional<Token> Parser::peek(int offset) const {
	if (m_index + offset >= m_tokens.size()) {
		return {};
	} else {
		return m_tokens.at(m_index + offset);
	}
};

Token Parser::consume() {
	return m_tokens.at(m_index++);
};