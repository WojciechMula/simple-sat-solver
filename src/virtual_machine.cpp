#include <assert.h>
#include "virtual_machine.h"

#include <cstdio>


namespace ssat {
	namespace vm {

		instruction_type::instruction_type(
			opcode_type 		opcode,
			register_index_type target,
			argument_source     arg1_source,
			register_index_type arg1,
			argument_source     arg2_source,
			register_index_type arg2
		) : opcode(opcode),
		    target(target),
			arg1(arg1),
			arg2(arg2),
			arg1_source(arg1_source),
			arg2_source(arg2_source) {

			// nop
		}

		instruction_type instruction_type::create_and(
			register_index_type target,
			argument_source     arg1_source,
			register_index_type arg1,
			argument_source     arg2_source,
			register_index_type arg2
		) {
			return instruction_type(AND, target, arg1_source, arg1, arg2_source, arg2);
		}


		instruction_type instruction_type::create_or(
			register_index_type target,
			argument_source     arg1_source,
			register_index_type arg1,
			argument_source     arg2_source,
			register_index_type arg2
		) {

			return instruction_type(OR, target, arg1_source, arg1, arg2_source, arg2);
		}


		instruction_type instruction_type::create_not(
			register_index_type target,
			argument_source     arg1_source,
			register_index_type arg1
		) {
			return instruction_type(NOT, target, arg1_source, arg1, UNUSED, 0);
		}


		void instruction_type::dump() {
			printf("T[%d] := ", target);
			switch (opcode) {
				case NOT:
					printf("not ");
					dump_index(arg1_source, arg1);
					break;

				case AND:
					dump_index(arg1_source, arg1);
					printf(" and ");
					dump_index(arg2_source, arg2);
					break;

				case OR:
					dump_index(arg1_source, arg1);
					printf(" or ");
					dump_index(arg2_source, arg2);
					break;
			}

			putchar('\n');
		}

		void instruction_type::dump_index(argument_source source, register_index_type index) {
			if (source == INPUT) {
				printf("I[%d]", index);
			} else {
				printf("T[%d]", index);
			}

		}

		void program_type::add(instruction_type instr) {

			temporary.insert(instr.target);
			if (instr.arg1_source == INPUT) {
				input.insert(instr.arg1);
			} else {
				temporary.insert(instr.arg1);
			}

			if (instr.arg2_source == INPUT) {
				input.insert(instr.arg2);
			} else {
				temporary.insert(instr.arg2);
			}

			instructions.push_back(instr);
		}


		const program_type::instructions_type& program_type::get_all() const {
			// assert(!input.empty())
			// assert(!temporary.empty())
			// assert(input.size() == max(input) - 1)
			// assert(temporary.size() == max(temporary) - 1)

			return instructions;
		}


		void program_type::dump() {
			for (auto i=0u; i < instructions.size(); i++) {
				printf("%d: ", i);
				instructions[i].dump();
			}
		}


		std::size_t program_type::input_size() const {
			return input.size();
		}


		std::size_t program_type::temporary_size() const {
			return temporary.size();
		}


		machine::machine(const program_type& prog) {
			const auto& instructions = prog.get_all();

			input_size = prog.input_size();
			temporary_size = prog.temporary_size();

			registers.resize(input_size + temporary_size);

			// transcode
			for (auto i=0u; i < instructions.size(); i++) {
				const auto instr = instructions[i];

				internal_instruction_type transcoded;

				transcoded.opcode = instr.opcode;
				transcoded.target = instr.target;

				if (instr.arg1_source == INPUT) {
					transcoded.arg1 = instr.arg1;
				} else {
					transcoded.arg1 = instr.arg1 + input_size;
				}

				if (instr.arg2_source == INPUT) {
					transcoded.arg2 = instr.arg2;
				} else {
					transcoded.arg2 = instr.arg2 + input_size;
				}

				program.push_back(transcoded);
			}
		}


		void machine::execute() {
			for (auto i=0u; i < program.size(); i++) {
				const auto instr = program[i];
				switch (instr.opcode) {
					case NOT: {
							const auto arg = get(instr.arg1);
							set_temporary(instr.target, ~arg);
							break;
						}

					case AND: {
							const auto arg1 = get(instr.arg1);
							const auto arg2 = get(instr.arg2);
							set_temporary(instr.target, arg1 & arg2);
							break;
						}

					case OR: {
							const auto arg1 = get(instr.arg1);
							const auto arg2 = get(instr.arg2);
							set_temporary(instr.target, arg1 | arg2);
							break;
						}
				}
			}
		}


		machine::value_type ssat::vm::machine::get(std::size_t index) const {
			assert(index < input_size + temporary_size);

			const auto reg = registers[index];

			assert(reg.initialized);

			return reg.value;
		}


		machine::value_type ssat::vm::machine::get_temporary(std::size_t index) const {
			assert(index < input_size + temporary_size);

			const auto reg = registers[index + input_size];

			assert(reg.initialized);

			return reg.value;
		}


		void machine::set_input(std::size_t index, ssat::vm::machine::value_type value) {
			assert(index < input_size);

			registers[index].initialized = true;
			registers[index].value = value;
		}


		void machine::set_temporary(std::size_t index, ssat::vm::machine::value_type value) {
			assert(index < temporary_size);

			registers[index + input_size].initialized = true;
			registers[index + input_size].value = value;
		}


		void machine::dump_memory() {
			for (std::size_t i=0u; i < registers.size(); i++) {
				if (i < input_size) {
					printf("R[%d]: ", i);
				} else {
					printf("T[%d]: ", i - input_size);
				}

				const auto& reg = registers[i];

				if (reg.initialized) {
					printf("%08x", reg.value);
				} else {
					printf("???");
				}

				putchar('\n');
			}
		}

	}
}

