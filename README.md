# SAT Solver Implementation with DPLL, CCL, and VSIDS Heuristics

This project implements a SAT solver using the DPLL algorithm with Conflict-Clause-Learning (CCL) and Variable State Independent Decaying Sum (VSIDS) heuristics in C++. It is designed to read a CNF file, solve the Boolean satisfiability problem, and print the results.

It also implements the basic DPLL and basic DPLL with Conflict-Clause-Learning (CCL) for benchmarking.

---

## Directory Structure

- **`DPLL_CCL_VSIDS.cpp`**: Contains DPLL algorithm with Conflict-Clause-Learning and VSIDS heuristics.
- **`DPLL_CCL.cpp`**: Contains DPLL algorithm with Conflict-Clause-Learning.
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

---

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

---

## Benchmarking Results

|Benchmark            |Variables|Clauses|Result|DPLL(s)   |CCL(s)  |VSIDS(s)|DPLL(KB)|CCL(KB)|VSIDS(KB)|
|---------------------|---------|-------|------|----------|--------|--------|--------|-------|---------|
|uf20-0941.cnf        |20       |91     |SAT   |0.000487  |0.00014 |6.1e-05 |3640    |2080   |2024     |
|uf20-0771.cnf        |20       |91     |SAT   |0.000237  |0.000222|0.000121|2072    |2080   |2016     |
|uf20-0114.cnf        |20       |91     |SAT   |0.000476  |0.000122|5.4e-05 |3608    |2068   |2144     |
|uf20-0681.cnf        |20       |91     |SAT   |0.000122  |0.000148|5.3e-05 |2024    |2024   |2068     |
|uf20-0518.cnf        |20       |91     |SAT   |0.000188  |0.000118|4.3e-05 |2084    |2072   |2068     |
|uf20-0159.cnf        |20       |91     |SAT   |0.000916  |0.000188|9.7e-05 |3684    |2020   |2024     |
|aim-50-2_0-yes1-4.cnf|50       |100    |SAT   |0.280276  |0.000325|0.000376|3868    |2024   |2020     |
|aim-50-3_4-yes1-4.cnf|50       |170    |SAT   |0.014847  |0.000273|0.000131|3952    |3676   |3568     |
|uuf50-0152.cnf       |50       |218    |UNSAT |0.117611  |0.003152|0.003082|4132    |3616   |3500     |
|uuf50-063.cnf        |50       |218    |UNSAT |0.035267  |0.003943|0.001621|4060    |3508   |3624     |
|uuf50-057.cnf        |50       |218    |UNSAT |0.01172   |0.003851|0.00195 |3868    |3512   |3560     |
|uf75-011.cnf         |75       |325    |SAT   |0.329479  |0.028254|0.00313 |4844    |3552   |3612     |
|uf75-01.cnf          |75       |325    |SAT   |0.005496  |0.002498|0.000487|4516    |3544   |3544     |
|uf75-012.cnf         |75       |325    |SAT   |0.463404  |0.003493|0.021155|4892    |3732   |3752     |
|uuf75-013.cnf        |75       |325    |UNSAT |-1.0      |0.039805|0.017947|4784    |3568   |3448     |
|flat30-100.cnf       |90       |300    |SAT   |0.000818  |0.000246|0.00073 |3792    |3556   |3460     |
|flat30-31.cnf        |90       |300    |SAT   |0.000879  |0.00024 |0.000475|3872    |3556   |3560     |
|flat30-83.cnf        |90       |300    |SAT   |0.000875  |0.000229|0.000809|3808    |3480   |3556     |
|flat30-45.cnf        |90       |300    |SAT   |0.000848  |0.000215|0.000479|3772    |3576   |3632     |
|uf100-01.cnf         |100      |430    |SAT   |5.525961  |0.135946|0.005175|5748    |3960   |3836     |
|uf100-035.cnf        |100      |430    |SAT   |2.635074  |0.106629|0.001393|5824    |3812   |3496     |
|uf100-0105.cnf       |100      |430    |SAT   |0.876672  |0.092339|0.001156|5384    |3828   |3896     |
|uf100-046.cnf        |100      |430    |SAT   |25.79496  |0.124762|0.003904|6068    |3820   |3860     |
|uuf125-027.cnf       |125      |538    |UNSAT |73.495758 |1.055077|0.904755|6748    |3996   |3824     |
|uuf125-064.cnf       |125      |538    |UNSAT |73.226721 |0.977241|0.160017|6268    |3744   |3824     |
|uuf150-067.cnf       |150      |645    |UNSAT |524.938348|3.795455|2.655511|7956    |4216   |4012     |
|uf150-01.cnf         |150      |645    |SAT   |53.603497 |0.24034 |0.038923|8100    |4076   |4080     |


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