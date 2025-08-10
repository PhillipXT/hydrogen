#include <iostream>
#include <fstream>
#include <sstream>

#include "../headers/generator.h"
#include "../headers/parser.h"
#include "../headers/tokenizer.h"

// Compile:		cmake -S . -B build/
// Build:		cmake --build build/
// Test:		build/hydro ./hydrogen/test.hy

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

	Tokenizer tokenizer(std::move(contents));

	std::vector<Token> tokens = tokenizer.tokenize();

	Parser parser(std::move(tokens));
	std::optional<NodeProgram> prog = parser.parse_program();

	if (!prog.has_value()) {
		std::cerr << "Invalid program." << std::endl;
		exit(EXIT_FAILURE);
	}

	Generator generator(prog.value());
	{
		std::fstream file("./output/out.asm", std::ios::out);
		file << generator.generate_program();
	}

	system("nasm -felf64 ./output/out.asm");
	system("ld -o ./output/out ./output/out.o");

	return EXIT_SUCCESS;
}