#ifndef ssat_test_lexer_h_included__
#define ssat_test_lexer_h_included__


#include <stdexcept>
#include <string>
#include <cstdint>


namespace test {

	class lexer_error : public std::logic_error {
		public:
			lexer_error(const char* msg)
				: std::logic_error(msg) {
			}
	};

	class lexer {
			std::string str;
			std::size_t index;

		public:
			static const int eof = -1;

		public:
			lexer(const std::string str);

			void reset();

		public:
			int peek();
			int get();
			void expect(int value);
			void expect_whitespaces();
			void expect_eof();
			void eat_spaces();
	};

}


#endif
