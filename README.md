# SAT Solver Implementation with DPLL, CDCL, and VSIDS Heuristics

This project implements a SAT solver using the DPLL algorithm with Conflict-Driven Clause Learning (CDCL) and Variable State Independent Decaying Sum (VSIDS) heuristics in C++. It is designed to read a CNF file, solve the Boolean satisfiability problem, and print the results.

It also implements the basic DPLL and basic DPLL with Conflict-Driven Clause Learning (CDCL) for benchmarking.

---

## Directory Structure

- **`DPLL_CDCL_VSIDS.cpp`**: Contains DPLL algorithm with Conflict-Driven Clause Learning and VSIDS heuristics.
- **`DPLL_CDCL.cpp`**: Contains DPLL algorithm with Conflict-Driven Clause Learning.
- **`DPLL.cpp`**: Contains basic DPLL algorithm only.
- **`benchmark_files/`**: This folder contains various CNF files for testing the SAT solver.
- **`benchmark.py`**: This is benchmarking script which compares the time taken and memory usage of the above three SAT solvers.
- **`graph_plot.py`**: Run this python program to plot the graph to visualise the comparison relative to basic DPLL.
- **`images/`**: This folder has benchmarking plots and other supporting images.
- **`run_all.sh`**: This shell script is used to run all cnf files from benchmark_files folder by selecting the required solver out of the three.
- **`DSDA_Project_Report_Group5.pdf`**: This PDF includes detailed documentation on the algorithm, changes in heuristics, results, and analysis.

---

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
   g++ -std=c++17 -O2 DPLL_CDCL_VSIDS.cpp -o DPLL_CDCL_VSIDS
   g++ -std=c++17 -O2 DPLL_CDCL.cpp -o DPLL_CDCL
   g++ -std=c++17 -O2 DPLL.cpp -o DPLL
```

4. Run the SAT solver by providing a CNF file as input:

   Example:
```bash
   ./DPLL_CDCL_VSIDS benchmark_files/aim-50-3_4-yes1-4.cnf
```

### Output

The program will print:

- **RESULT**: Indicating whether the result is `SAT` or `UNSAT`.
- **Assignments** (if SAT): Displays variable assignments.
- **Time taken** (if SAT): Displays the time taken to run the algorithm.
- **Memory used** (if SAT): Displays the memory used to run the algorithm.

---

## How to Benchmark

### Prerequisites

- **Python 3.12.3** (with pandas and matplotlib)

### Steps

1. In the same directory and terminal, execute the following:

```bash
   python3 benchmark.py
```

2. The above python code generates the benchmarking csv file. To plot the results from this csv file, run:

```bash
   python3 graph_plot.py
```

3. Graphs will be saved in ./images folder.

---

## Benchmarking Results

For benchmarked data on time taken and memory usage of the three SAT solvers on various cnf files, refer to the [CSV Table](solver_comparison_tabulated.csv).


### Time Efficiency Comparison
![Time Efficiency](images/performance_time_analysis.png)

### Memory Usage Comparison
![Memory Usage](images/performance_memory_analysis.png)

---

## Full Report

For detailed analysis, benchmarking methodology, and results, refer to the [Project Report (PDF)](DSDA_Project_Report_Group5.pdf).

---

## Contributors
- [Vishal Kevat](https://github.com/vishalkevat007)  
- [Adrija Chakraborty](https://github.com/Adrija-debug)