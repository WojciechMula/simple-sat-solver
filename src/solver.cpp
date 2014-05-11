#include "solver.h"
#include <assert.h>


namespace ssat {

	const solver::value_type solver::const_seq[solver::const_seq_size] = {
			0xaaaaaaaa, // 01010101010101010101010101010101
			0xcccccccc, // 00110011001100110011001100110011
			0xf0f0f0f0, // 00001111000011110000111100001111
			0xff00ff00, // 00000000111111110000000011111111
			0xffff0000  // 00000000000000001111111111111111
	};

	const solver::value_type solver::all_zeros = 0;
	const solver::value_type solver::all_ones  = 0xffffffff;


	solver::solver(const vm::program_type& program)
		: program(program),
		  machine(program) {

		// nop
	}

	solver::result_type solver::check() {
		if (program.input_size() <= solver::const_seq_size) {
			return check_small_set();
		} else {
			return check_large_set();
		}
	}


	solver::result_type solver::check_small_set() {
		for (auto i=0u; i < program.input_size(); i++) {
			machine.set_input(i, const_seq[i]);
		}

		machine.execute();

		const auto index = program.temporary_size() - 1;
		const auto result = machine.get_temporary(index);

		if (result == all_zeros) {
			return Unsatisfiable;
		} else if (result == all_ones) {
			return Tautology;
		} else {
			return Satisfiable;
		}
	}


	solver::result_type solver::check_large_set() {
		for (auto i=0u; i < program.input_size(); i++) {
			machine.set_input(i, const_seq[i]);
		}

		value_type any_set = all_zeros;
		value_type all_set = all_ones;

		const auto index = program.temporary_size() - 1;
		const auto combinations = 1u << (program.input_size() - const_seq_size);
		for (auto i=0u; i < combinations; i++) {

			// setup input variables above const_seq_size
			for (auto j=0u; j < program.input_size() - const_seq_size; j++) {
				if (i & (1u << j)) {
					machine.set_input(j + const_seq_size, all_ones);
				} else {
					machine.set_input(j + const_seq_size, all_zeros);
				}
			}

			machine.execute();

			const auto result = machine.get_temporary(index);

			any_set |= result;
			all_set &= result;
		}

		if (all_set == all_ones) {
			return Tautology;
		} else if (any_set != all_zeros) {
			return Satisfiable;
		} else {
			return Unsatisfiable;
		}
	}

	const char* result_name(solver::result_type result) {
		switch (result) {
			case solver::Satisfiable:
				return "satisfiable";

			case solver::Unsatisfiable:
				return "unsatisfiable";

			case solver::Tautology:
				return "tautology";

			default:
				return "<unknown>";
		}

	}

} // namespace ssat
