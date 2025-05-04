# SAT Solver

This project implements a SAT solver using the DPLL algorithm with Conflict-Clause-Learning and VSIDS heuristics. It is designed to read a CNF file, solve the Boolean satisfiability problem, and print the results.

## Directory Structure

- **`DPLL_CCL_VSIDS.cpp`**: Contains DPLL algorithm with Conflict-Clause-Learning and VSIDS heuristics.
- **`DPLL_CCL.cpp`**: Contains DPLL algorithm with Conflict-Clause-Learning.
- **`DPLL.cpp`**: Contains basic DPLL algorithm only.
- **`benchmark_files/`**: This folder contains various CNF files for testing the SAT solver.
- **`benchmark.py`**: This is benchmarking script which compares the time taken and memory usage of the above three SAT solvers.
- **`graph_plot.py`**: Run this python program to plot the graph to visualise the comparison relative to basic DPLL.
- **`DSDA_Project_Report_Group5.pdf`**: This PDF includes detailed documentation on the algorithm, changes in heuristics, results, and analysis.

## How to Run

### Prerequisites

- **C++ Compiler:** `g++` (GCC for Linux/macOS, MinGW for Windows)

### Steps

1. Open a terminal or command prompt.

2. Navigate to the project directory:

```bash
   cd Group5_Cpp
```

3. Compile the SAT solver using the following command:

```bash
   g++ -std=c++17 -O2 DPLL_CCL_VSIDS.cpp -o DPLL_CCL_VSIDS
   g++ -std=c++17 -O2 DPLL_CCL.cpp -o DPLL_CCL
   g++ -std=c++17 -O2 DPLL.cpp -o DPLL
```

4. Run the SAT solver by providing a CNF file as input:

   Example:
```bash
   ./DPLL_CCL_VSIDS benchmark_files/aim-50-3_4-yes1-4.cnf
```

### Output

The script will print:

- **RESULT**: Indicating whether the result is `SAT` or `UNSAT`.
- **Assignments** (if SAT): Displays variable assignments.