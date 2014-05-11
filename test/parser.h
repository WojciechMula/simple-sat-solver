#ifndef ssat_test_parser_h_included__
#define ssat_test_parser_h_included__


#include "ast.h"
#include "lexer.h"
#include "../src/virtual_machine.h"

namespace test {

	ast::node_t* create(lexer& lexer);

} // namespace test


#endif
