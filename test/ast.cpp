#include "ast.h"
#include <cstdio>

namespace test {

	namespace ast {

		void destroy(node_t* root) {
			if (root == nullptr) {
				return;
			}

			destroy(root->left);
			destroy(root->right);

			delete root;
		}


		void dump_aux(node_t* root) {
			using namespace std;

			switch (root->type) {
				case VARIABLE:
					printf("%c", root->value);
					break;

				case NOT:
					printf("not (");
					dump_aux(root->left);
					printf(")");
					break;

				case AND:
					printf("(");
					dump_aux(root->left);
					printf(") and (");
					dump_aux(root->right);
					printf(")");
					break;

				case OR:
					printf("(");
					dump_aux(root->left);
					printf(") or (");
					dump_aux(root->right);
					printf(")");
					break;
			}
		}


		void dump(node_t* root) {
			dump_aux(root);
			printf("\n");
		}


		compiler::compiler(node_t* root)
			: root(root) {

			numerate_nodes(root);
		}


		void compiler::numerate_nodes(node_t* node) {
			if (node == nullptr)
				return;

			numerate_nodes(node->left);
			numerate_nodes(node->right);

			if (node->type != VARIABLE && node_numbers.count(node) == 0) {
				const auto n = node_numbers.size();
				node_numbers[node] = n;
			}
		}


		void compiler::compile(ssat::vm::program_type& program) {
			compile_aux(root, program);
		}


		void compiler::compile_aux(node_t* root, ssat::vm::program_type& program) {
			if (root == nullptr) {
				return;
			}

			compile_aux(root->left, program);
			compile_aux(root->right, program);

			switch (root->type) {
				case NOT:
					program.add(
						ssat::vm::instruction_type::create_not(
							get_node_number(root),
							get_argument_source(root->left),
							get_argument_index(root->left)
						)
					);
					break;

				case AND:
					program.add(
						ssat::vm::instruction_type::create_and(
							get_node_number(root),
							get_argument_source(root->left),
							get_argument_index(root->left),
							get_argument_source(root->right),
							get_argument_index(root->right)
						)
					);
					break;

				case OR:
					program.add(
						ssat::vm::instruction_type::create_or(
							get_node_number(root),
							get_argument_source(root->left),
							get_argument_index(root->left),
							get_argument_source(root->right),
							get_argument_index(root->right)
						)
					);
					break;

				case VARIABLE:
					break;
			}
		}

		ssat::vm::argument_source compiler::get_argument_source(node_t* node) {
			if (node->type == VARIABLE) {
				return ssat::vm::argument_source::INPUT;
			} else {
				return ssat::vm::argument_source::TEMPORARY;
			}
		}


		int compiler::get_argument_index(node_t* node) {
			if (node->type == VARIABLE) {
				return node->value - 'A';
			} else {
				return get_node_number(node);
			}
		}


		int compiler::get_node_number(node_t* node) {
			return node_numbers[node];
		}
	}

} // namespace test

