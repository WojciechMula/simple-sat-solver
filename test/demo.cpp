#include "../src/solver.h"
#include "parser.h"
#include <cstdio>


struct options_type {
	bool show_input;
	bool show_parsed;
	bool dump_program;
};


void compile_and_check(std::string expression, const options_type& options) {
	if (options.show_input) {
		std::printf("raw expression: %s\n", expression.c_str());
	}

	ssat::vm::program_type program;

	{
		test::lexer lexer(expression);
		auto tree = test::create(lexer);

		test::ast::compiler compiler(tree);
		compiler.compile(program);

		if (options.show_parsed) {
			printf("parsed: ");
			test::ast::dump(tree);
		}
		test::ast::destroy(tree);
	}

	if (options.dump_program) {
		program.dump();
	}

	ssat::solver solver(program);
	std::printf("result: %s\n", ssat::result_name(solver.check()));
}


int main(int argc, char* argv[]) {
	options_type options;

	options.show_input   = true;
	options.show_parsed  = true;
	options.dump_program = false;

	for (auto i=1; i < argc; i++) {
		compile_and_check(argv[i], options);
	}

	return 0;
}


