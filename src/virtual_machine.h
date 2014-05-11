#ifndef ssat_virtual_machine_h_included__
#define ssat_virtual_machine_h_included__


#include <vector>
#include <set>
#include <cstdint>


namespace ssat {

	namespace vm {

		typedef int register_index_type;

		enum opcode_type {
			NOT,
			AND,
			OR
		};

		enum argument_source {
			UNUSED,
			INPUT,
			TEMPORARY
		};

		class instruction_type {
			public:
				opcode_type         opcode;
				register_index_type target;
				register_index_type arg1;
				register_index_type arg2;

				argument_source arg1_source;
				argument_source arg2_source;

			private:
				instruction_type(
					opcode_type 		opcode,
					register_index_type target,
					argument_source     arg1_source,
					register_index_type arg1,
					argument_source     arg2_source,
					register_index_type arg2
				);

			public:
				static instruction_type create_and(
					register_index_type target,
					argument_source     arg1_source,
					register_index_type arg1,
					argument_source     arg2_source,
					register_index_type arg2
				);

				static instruction_type create_or(
					register_index_type target,
					argument_source     arg1_source,
					register_index_type arg1,
					argument_source     arg2_source,
					register_index_type arg2
				);

				static instruction_type create_not(
					register_index_type target,
					argument_source     arg1_source,
					register_index_type arg1
				);

				void dump();
				void dump_index(argument_source source, register_index_type index);
		};

		class program_type {
			public:
				typedef std::vector<instruction_type> instructions_type;

			private:
				instructions_type instructions;

				std::set<register_index_type> input;
				std::set<register_index_type> temporary;

			public:
				void add(instruction_type instruction);

				const instructions_type& get_all() const;

				std::size_t input_size() const;
				std::size_t temporary_size() const;
				std::size_t result_index() const;

				void dump();
		};


		class machine {
			public:
				typedef uint32_t value_type;

			private:
				struct internal_instruction_type {
					opcode_type opcode;
					register_index_type target;
					register_index_type arg1;
					register_index_type arg2;
				};

				struct register_type {
					bool initialized;
					value_type value;

					register_type() : initialized(false) {
						// nop
					}
				};

				std::vector<register_type> registers;
				std::vector<internal_instruction_type> program;
				std::size_t input_size;
				std::size_t temporary_size;

			public:
				machine(const program_type& program);
				void execute();

				void set_input(std::size_t index, value_type value);
				value_type get_temporary(std::size_t index) const;


			private:
				value_type get(std::size_t index) const;
				void set_temporary(std::size_t index, value_type value);

				void dump_memory();
		};

	} // namepace vm

} // namepace ssat


#endif
