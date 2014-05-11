#ifndef ssat_solver_h_included
#define ssat_solver_h_included


#include "virtual_machine.h"
#include <assert.h>


namespace ssat {

	class solver {
		private:
			typedef std::uint32_t value_type;

		private:
			static const std::size_t const_seq_size = 5;
			static const value_type const_seq[const_seq_size];

			static const value_type all_zeros;
			static const value_type all_ones;

		public:
			enum result_type {
				Unsatisfiable,
				Satisfiable,
				Tautology
			};

			const vm::program_type& program;
			vm::machine machine;


		public:
			solver(const vm::program_type& program);

			result_type check();

		private:
			result_type check_small_set();
			result_type check_large_set();
	};

	const char* result_name(solver::result_type result);
}

#endif
