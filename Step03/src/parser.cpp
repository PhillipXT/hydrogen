#include <iostream>

#include "../headers/parser.h"

std::optional<NodeExpression> Parser::parse_expr() {
	if (peek().has_value() && peek().value().type == TokenType::int_lit) {
		return NodeExpression { .var = NodeExpressionIntegerLiteral { .int_lit = consume() }};
	} else if (peek().has_value() && peek().value().type == TokenType::identifier) {
		return NodeExpression { .var = NodeExpressionIdentifier { .identifier = consume() }};
	} else {
		return {};
	}
};

std::optional<NodeStatement> Parser::parse_stmt() {
	if (peek().value().type == TokenType::exit && peek(1).has_value() && peek(1).value().type == TokenType::open_paren) {
		consume();	// Consume the [exit] token
		consume();	// Consume the [(] token
		NodeStatementExit stmt_exit;
		if (auto node_expr = parse_expr()) {
			stmt_exit = { .expr = node_expr.value() };
		} else {
			std::cerr << "Invalid expression" << std::endl;
			exit(EXIT_FAILURE);
		}
		if (peek().has_value() && peek().value().type == TokenType::close_paren) {
			consume();
		} else {
			std::cerr << "Syntax error: expected ')'" << std::endl;
			exit(EXIT_FAILURE);
		}
		if (peek().has_value() && peek().value().type == TokenType::semicolon) {
			consume();
		} else {
			std::cerr << "Syntax error: expected ';'" << std::endl;
			exit(EXIT_FAILURE);
		}
		return NodeStatement { .var = stmt_exit };
	} else if (peek().has_value() && peek().value().type == TokenType::var
		&& peek(1).has_value() && peek(1).value().type == TokenType::identifier
		&& peek(2).has_value() && peek(2).value().type == TokenType::eq) {
			consume();	// Consume the [var] token
			auto stmt_var = NodeStatementVar { .ident = consume() };
			consume();	// Consume the [=] token
			if (auto expr = parse_expr()) {
				stmt_var.expr = expr.value();
			} else {
				std::cerr << "Invalid expression" << std::endl;
				exit(EXIT_FAILURE);
			}
			if (peek().has_value() && peek().value().type == TokenType::semicolon) {
				consume();
			} else {
				std::cerr << "Syntax error: expected ';'" << std::endl;
				exit(EXIT_FAILURE);
			}
			return NodeStatement { .var = stmt_var };
	} else {
		return {};
	}
};

std::optional<NodeProgram> Parser::parse_program() {
	NodeProgram prog;
	while (peek().has_value()) {
		if (auto stmt = parse_stmt()) {
			prog.statements.push_back(stmt.value());
		} else {
			std::cerr << "Invalid statement" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	return prog;
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