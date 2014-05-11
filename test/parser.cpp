#include "parser.h"

namespace test {

	ast::node_t* expression(test::lexer& lexer);

	ast::node_t* create(test::lexer& lexer) {
		lexer.reset();

		auto result = expression(lexer);

		lexer.expect_eof();

		return result;
	}

	ast::node_t* expression(test::lexer& lexer) {
		lexer.eat_spaces();

		auto letter = lexer.get();
		switch (letter) {
			case 'a': { // and
				lexer.expect('n');
				lexer.expect('d');

				lexer.expect_whitespaces();
				auto arg1 = expression(lexer);
				lexer.expect_whitespaces();
				auto arg2 = expression(lexer);

				return new ast::node_t(ast::AND, arg1, arg2);
			}

			case 'o': { // or
				lexer.expect('r');

				lexer.expect_whitespaces();
				auto arg1 = expression(lexer);
				lexer.expect_whitespaces();
				auto arg2 = expression(lexer);

				return new ast::node_t(ast::OR, arg1, arg2);
			}

			case 'n': { // not
				lexer.expect('o');
				lexer.expect('t');

				lexer.expect_whitespaces();
				auto arg = expression(lexer);

				return new ast::node_t(ast::NOT, arg);
			}

			default:
				if (std::isupper(letter)) {
					return new ast::node_t(letter);
				} else {
					throw std::logic_error("unexpected character");
				}
		}
	}
}
