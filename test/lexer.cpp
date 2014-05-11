#include "lexer.h"
#include <cctype>


namespace test {

	lexer::lexer(std::string str)
		: str(str),
		  index(0) {

		// nop
	}


	void lexer::reset() {
		index = 0;
	}


	int lexer::peek() {
		if (index < str.size()) {
			return str[index];
		} else if (index == str.size()) {
			return eof;
		}

		throw lexer_error("unexpected end of string");
	}


	int lexer::get() {
		const int result = peek();

		index += 1;

		return result;
	}


	void lexer::eat_spaces() {
		while (std::isspace(peek())) {
			index += 1;
		}
	}


	void lexer::expect(int value) {
		if (get() != value) {
			throw lexer_error("expected value");
		}
	}


	void lexer::expect_whitespaces() {
		int n = 0;
		while (std::isspace(peek())) {
			index += 1;
			n += 1;
		}

		if (n == 0) {
			throw lexer_error("expected at least one space");
		}
	}


	void lexer::expect_eof() {
		const auto letter = get();
		if (letter != eof) {
			throw lexer_error("expected eof");
		}
	}

}
