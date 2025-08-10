#pragma once

#include <unordered_map>

#include "parser.h"

class Generator {
	public:
		explicit Generator(NodeProgram program) : m_prog(std::move(program)) {};
		void generate_expression(const NodeExpression& expr);
		void generate_statement(const NodeStatement& stmt);
		[[nodiscard]] std::string generate_program();
	private:
		struct Var {
			size_t stack_location;
		};
		void push(const std::string& reg);
		void pop(const std::string& reg);
		const NodeProgram m_prog;
		std::stringstream m_output;
		size_t m_stack_size = 0;
		std::unordered_map<std::string, Var> m_vars {};
};