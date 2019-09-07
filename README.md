# Operation-Systems---Processes
A program that checks Sudoku if Sudoku solutions are valid.

The program was written and compiled in Linux.

The master process forks into child proceseses, who are opening a new program(Check.c).
The master process then sends information to its children, who, in turn, check if a Sudoku solution is valid or not.
The results are returned to the master process, who informs a final verdict.


v1 works using pipes to communicate between the processes.

v2 works using shared memory for all processes.
