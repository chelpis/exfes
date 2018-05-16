(1) API Arguments

  The main function "int exfes(...)" to solve MQ is declared in "exfes.h".

  (numFixedVariables, numVariables, numEquations) are the parameters for the MQ
  system. You may use (0, 56, 48), (1, 64, 56), (9, 72, 64), (17, 80, 72).

  (startPointHigh, startPointLow) represents the start point of the solving
  process. You may use random numbers.
  
  (coefficientsMatrix) is the pointer to the MQ system coefficients. Only 0 or
  1 are allowed coefficients.

  (shouldAbortNow) is the pointer to a function provided by users. The solver
  will call this function during the solving process, and abort if this
  function returns true. You may implement this function to check if other
  nodes have found a solution or not.

  (solutionHigh, solutionLow) is the pointer to the solution buffer. If the
  solver finds a solution, the solution will be written into this buffer. 


(2) API Returning Values

  Returning 0 means the solver found a solution, and write the solution into
  the solution buffer.

  Returning -1 means the solver searched all possibilities but no solution was
  found.

  Returning -2 means the solver is interrupted by the shouldAbortNow function.

  Returning -3 means the input parameters are invalid.

  Returning -4 means an memory allocation failed for some reasons, and is took
  care by freeing unused memory.


(3) Directory Structure

  Three directories are in this repository.

  All source and header files provided for the MQ solver are under lib/.

  Examples to use the MQ solver are under sample/.

  Long running tests for correctness and reliability are under tests/.


(4) How to Build

  Simply "make" shall build all executables.


