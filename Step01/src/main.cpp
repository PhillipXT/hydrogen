#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>

// Compile:		cmake -S . -B build/
// Build:		cmake --build build/
// Test:		build/hydro ./hydrogen/test_01.hy

enum class TokenType {
	exit,
	int_lit,
	semicolon
};

struct Token {
	TokenType type;
	std::optional<std::string> value;
};

std::vector<Token> tokenize(const std::string& str) {
	std::vector<Token> tokens;
	std::string buffer;
	for (int i = 0; i < str.length(); i++) {
		char c = str.at(i);
		if (std::isalpha(c)) {
			buffer.push_back(c);
			i++;
			while (std::isalnum(str.at(i))) {
				buffer.push_back(str.at(i));
				i++;
			}
			i--;
			if (buffer == "exit") {
				tokens.push_back({.type = TokenType::exit});
				buffer.clear();
				continue;
			} else {
				std::cerr << "You messed up! (1)" << std::endl;
				exit(EXIT_FAILURE);
			}
		} else if (std::isdigit(c)) {
			buffer.push_back(c);
			i++;
			while (std::isdigit(str.at(i))) {
				buffer.push_back(str.at(i));
				i++;
			}
			i--;
			tokens.push_back({.type = TokenType::int_lit, .value = buffer});
			buffer.clear();
		} else if (c == ';') {
			tokens.push_back({.type = TokenType::semicolon});
		} else if (std::isspace(c)) {
			continue;
		} else {
			std::cerr << "You messed up! (2)" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	return tokens;
}

std::string tokens_to_assembly(const std::vector<Token>& tokens) {
	std::stringstream output;
	output << "global _start\n_start:\n";
	for (int i = 0; i < tokens.size(); i++) {
		const Token& token = tokens.at(i);
		if (token.type == TokenType::exit) {
			if (i + 1 < tokens.size() && tokens.at(i + 1).type == TokenType::int_lit) {
				if (i + 2 < tokens.size() && tokens.at(i + 2).type == TokenType::semicolon) {
					output << "    mov rax, 60\n";
					output << "    mov rdi, " << tokens.at(i + 1).value.value() << "\n";
					output << "    syscall";
				}
			}
		}
	}
	return output.str();
}

int main(int argc, char** argv) {

	// argv[0] is the current path/executable
	if (argc != 2) {
		std::cerr << "Incorrect usage" << std::endl;
		std::cerr << "hydro <input.hy>" << std::endl;
		return EXIT_FAILURE;
	}

	std::string contents;
	{
		std::stringstream contents_stream;
		std::fstream input(argv[1], std::ios::in);
		contents_stream << input.rdbuf();
		contents = contents_stream.str();
	}

	std::cout << contents << std::endl;

	std::vector<Token> tokens = tokenize(contents);

	{
		std::fstream file("./output/out.asm", std::ios::out);
		file << tokens_to_assembly(tokens);
	}

	system("nasm -felf64 ./output/out.asm");
	system("ld -o ./output/out ./output/out.o");

	return EXIT_SUCCESS;
}