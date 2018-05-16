### API Arguments

The main function "int exfes(...)" to solve MQ is declared in "exfes.h".

The tuple (numFixedVariables, numVariables, numEquations) is the parameter for
the MQ system.  For example: (0, 56, 48), (1, 64, 56), (9, 72, 64), (17, 80,
72).

The pair (startPointHigh, startPointLow) represents the start point of the
solving process.  You may use random numbers.

The variable (coefficientsMatrix) represents the array of coefficients in the
MQ system.  Each array element must be 0 or 1.

The variable (shouldAbortNow) is a function pointer provided by the user.  The
solver will call this function during the solving process, and abort if this
function returns true.  User should implement this function to check if some
other node or thread has already found a solution.

The pair (solutionHigh, solutionLow) represents the solution buffer.  If the
solver finds a solution, the solution will be written here.



### API Returning Values

Returning 0 means the solver found a solution, and wrote the solution into the
solution buffer.

Returning -1 means the solver searched all possibilities but no solution was
found.

Returning -2 means the solver was interrupted by the shouldAbortNow function.

Returning -3 means the some input parameters are invalid.

Returning -4 means a dynamic memory allocation failure; this rarely happens;
the caller could retry the computation.



### Directory Structure

Three directories are in this repository.

All source and header files for the MQ solver are under lib/.

Examples to use the MQ solver are under sample/.

Testing programs/scripts are under tests/.



### How to Build

Simply "make" shall build all executables.
