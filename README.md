# SAT Solver

This project implements a SAT solver using the DPLL algorithm with Conflict-Clause-Learning (CCL) and Variable State Independent Decaying Sum (VSIDS) heuristics. It is designed to read a CNF file, solve the Boolean satisfiability problem, and print the results.

It also implements the basic DPLL and basic DPLL with Conflict-Clause-Learning (CCL) for benchmarking.

## Directory Structure

- **`DPLL_CCL_VSIDS.cpp`**: Contains DPLL algorithm with Conflict-Clause-Learning and VSIDS heuristics.
- **`DPLL_CCL.cpp`**: Contains DPLL algorithm with Conflict-Clause-Learning.
- **`DPLL.cpp`**: Contains basic DPLL algorithm only.
- **`benchmark_files/`**: This folder contains various CNF files for testing the SAT solver.
- **`benchmark.py`**: This is benchmarking script which compares the time taken and memory usage of the above three SAT solvers.
- **`graph_plot.py`**: Run this python program to plot the graph to visualise the comparison relative to basic DPLL.
- **`images/`**: This folder has benchmarking plots and other supporting images.
- **`run.sh`**: This shell is used to run all cnf files from benchmark_files folder by selecting the required solver out of the three.
- **`DSDA_Project_Report_Group5.pdf`**: This PDF includes detailed documentation on the algorithm, changes in heuristics, results, and analysis.

## How to Run

### Prerequisites

- **C++ Compiler:** `g++` (GCC for Linux/macOS, MinGW for Windows)

### Steps

1. Open a terminal or command prompt.

2. Navigate to the project directory:

```bash
   git clone https://github.com/vishalkevat007/SAT-Solver-Implementation.git
   cd SAT-Solver-Implementation
```

3. Compile the three SAT solver using the following command:

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

The program will print:

- **RESULT**: Indicating whether the result is `SAT` or `UNSAT`.
- **Assignments** (if SAT): Displays variable assignments.
- **Time taken** (if SAT): Displays the time taken to run the algorithm.
- **Memory used** (if SAT): Displays the memory used to run the algorithm.


## How to Benchmark

### Prerequisites

- **Python 3.12.3** (with pandas and matplotlib)

### Steps

1. In the same directory and terminal, execute the following:

```bash
   python benchmark.py
```

2. The above python code generates the benchmarking csv file. To plot the results from this csv file, run:

```bash
   python graph_plot.py
```

3. Graphs will be saved in ./images folder.