#include "../src/solver.h"
#include <stdio.h>
#include "parser.h"


void check(std::string expression, ssat::solver::result_type expected_result) {
	ssat::vm::program_type program;

	{
		test::lexer lexer(expression);
		auto tree = test::create(lexer);

		test::ast::compiler compiler(tree);
		compiler.compile(program);

		test::ast::dump(tree);
		test::ast::destroy(tree);
	}

	ssat::solver solver(program);
	auto result = solver.check();

	if (expected_result != result) {
		printf("\tERROR: result is %s but should be %s\n",
			ssat::result_name(result),
			ssat::result_name(expected_result)
		);
	} else {
		printf("\tOK\n");
	}
}


int main() {
	// equivalence:
	// (A and B) or (not A and not B)
	check("or and A B and not A not B", ssat::solver::Satisfiable);

	// deMorgan law:
	// not (A and B) <=> not A or not B
	check("or and not not and A B not or not A not B and not and A B or not A not B", ssat::solver::Tautology);

	// A and not A
	check("and A not A", ssat::solver::Unsatisfiable);

	// many variables
	check("and and and and and and and and and A B C D E F G H I J", ssat::solver::Satisfiable);

	return 1;
}

