========================================================================
                        Simple SAT Solver
========================================================================


Introduciton
------------------------------------------------------------------------

This is simple SAT solver for tiny problems, up to 10-15 variables.

I tried to make this solver AST-agnostic, so input for solver is
a program containing AND, OR and NOT instructions. Please refer to
`test/test.cpp` and `test/demo.cpp` how to prepare data.

Sample session with `test/demo`::

	# expression are given in prefix notation
	$ ./demo "and A not A"
	raw expression: and A not A
	parsed: (A) and (not (A))
	result: unsatisfiable

	$ ./demo "or A not A"
	raw expression: or A not A
	parsed: (A) or (not (A))
	result: tautology

	$ ./demo "and or A B A"
	raw expression: and or A B A
	parsed: ((A) or (B)) and (A)
	result: satisfiable

	$ ./demo "and and or A B A not B"
	raw expression: and and or A B A not B
	parsed: (((A) or (B)) and (A)) and (not (B))
	result: satisfiable


License
------------------------------------------------------------------------

Public domain
