#include <iostream>
#include <sstream>

#include "../headers/generator.h"

void Generator::generate_expression(const NodeExpression& expr) {
	struct ExpressionVisitor {
		Generator* gen;
		void operator()(const NodeExpressionIdentifier& expr_id) const {
			if (!gen->m_vars.contains(expr_id.identifier.value.value())) {
				std::cerr << "Undeclared identifier: " << expr_id.identifier.value.value() << std::endl;
				exit(EXIT_FAILURE);
			}
			const auto& var = gen->m_vars.at(expr_id.identifier.value.value());
			std::stringstream offset;
			offset << "QWORD [rsp + " << (gen->m_stack_size - var.stack_location - 1) * 8 << "]";
			gen->push(offset.str());
		}
		void operator()(const NodeExpressionIntegerLiteral& expr_int_lit) const {
			gen->m_output << "    mov rax, " << expr_int_lit.int_lit.value.value() << "\n";
			gen->push("rax");
		}
	};
	ExpressionVisitor visitor { .gen = this };
	std::visit(visitor, expr.var);
}

void Generator::generate_statement(const NodeStatement& stmt) {
	struct StatementVisitor {
		Generator* gen;
		void operator()(const NodeStatementExit& stmt_exit) const {
			gen->generate_expression(stmt_exit.expr);
			gen->m_output << "    mov rax, 60\n";
			gen->pop("rdi");
			gen->m_output << "    syscall\n";
		}
		void operator()(const NodeStatementVar& stmt_var) const {
			if (gen->m_vars.contains(stmt_var.ident.value.value())) {
				std::cerr << "Identifier already used: " << stmt_var.ident.value.value() << std::endl;
				exit(EXIT_FAILURE);
			}
			gen->m_vars.insert({stmt_var.ident.value.value(), Var { .stack_location = gen->m_stack_size }});
			gen->generate_expression(stmt_var.expr);
		}
	};
	StatementVisitor visitor { .gen = this };
	std::visit(visitor, stmt.var);
};

[[nodiscard]] std::string Generator::generate_program() {
	m_output << "global _start\n_start:\n";

	for (const NodeStatement& stmt : m_prog.statements) {
		generate_statement(stmt);
	}

	m_output << "    mov rax, 60\n";
	m_output << "    mov rdi, 0\n";
	m_output << "    syscall\n";
	return m_output.str();
};

void Generator::push(const std::string& reg) {
	m_output << "    push " << reg << "\n";
	m_stack_size++;
}

void Generator::pop(const std::string& reg) {
	m_output << "    pop " << reg << "\n";
	m_stack_size--;
}