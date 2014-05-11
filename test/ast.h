#ifndef ssat_test_ast_h_included__
#define ssat_test_ast_h_included__


#include "../src/virtual_machine.h"
#include <map>


namespace test {

	namespace ast {

		enum node_type {
			AND,
			OR,
			NOT,
			VARIABLE
		};

		struct node_t {
			node_type   type;
			node_t* 	left;
			node_t* 	right;
			int         value;

			node_t(node_type type, node_t* left, node_t* right = nullptr)
				: type(type),
				  left(left),
				  right(right) {

				// nop
			}

			node_t(int value)
				: type(VARIABLE),
				  left(nullptr),
				  right(nullptr),
				  value(value) {

				// nop
			}
		};

		void destroy(node_t* root);
		void dump(node_t* root);

		class compiler {
			private:
				node_t* root;
				std::map<node_t*, int> node_numbers;

			public:
				compiler(node_t* root);

				void compile(ssat::vm::program_type& program);

			private:
				void numerate_nodes(node_t* node);
				void compile_aux(node_t* root, ssat::vm::program_type& program);
				ssat::vm::argument_source get_argument_source(node_t* node);
				int get_argument_index(node_t* node);
				int get_node_number(node_t* node);
		};
	}

} // namespace test


#endif
