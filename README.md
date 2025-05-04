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
- **`run.sh`**: This shell is used to run all cnf files from benchmark_files folder by selecting the required solver out of the three.
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

|Benchmark            |Variables|Clauses|Result|DPLL (s)   |CCL (s)  |VSIDS (s)|DPLL (KB)|CCL (KB)|VSIDS (KB)|
|---------------------|---------|-------|------|-----------|---------|---------|---------|--------|----------|
|uf20-0941.cnf        |20       |91     |SAT   |0.0004669  |0.0002379|6.11e-05 |2084     |2024    |2024      |
|uf20-0771.cnf        |20       |91     |SAT   |0.0002251  |0.0002208|0.0001205|3620     |2080    |2072      |
|uf20-0114.cnf        |20       |91     |SAT   |0.0005037  |0.0001189|5.43e-05 |3644     |2020    |2024      |
|uf20-0681.cnf        |20       |91     |SAT   |0.0001032  |0.0001321|4.4e-05  |2024     |2028    |2024      |
|uf20-0518.cnf        |20       |91     |SAT   |0.000201   |0.0001179|4.22e-05 |2076     |2028    |2072      |
|uf20-0159.cnf        |20       |91     |SAT   |0.0009433  |0.0002089|9.55e-05 |3568     |2020    |2080      |
|aim-50-2_0-yes1-4.cnf|50       |100    |SAT   |0.2779055  |0.0003447|0.000404 |3900     |1996    |2072      |
|aim-50-3_4-yes1-4.cnf|50       |170    |SAT   |0.0152071  |0.000303 |0.0001308|3944     |3496    |3628      |
|uuf50-0152.cnf       |50       |218    |UNSAT |0.1162799  |0.0033504|0.0030548|4228     |3408    |3512      |
|uuf50-063.cnf        |50       |218    |UNSAT |0.0347432  |0.0040153|0.0023096|4188     |3500    |3628      |
|uuf50-057.cnf        |50       |218    |UNSAT |0.0119384  |0.0040231|0.002085 |3808     |3504    |3416      |
|uf75-011.cnf         |75       |325    |SAT   |0.3556642  |0.02858  |0.0031022|4840     |3512    |3616      |
|uf75-01.cnf          |75       |325    |SAT   |0.0052017  |0.0018016|0.0005337|4496     |3536    |3496      |
|uf75-012.cnf         |75       |325    |SAT   |0.4593895  |0.0036201|0.0217276|4812     |3760    |3896      |
|uuf75-013.cnf        |75       |325    |UNSAT |0.8189278  |0.0399889|0.0180194|4796     |3596    |3628      |
|flat30-100.cnf       |90       |300    |SAT   |0.0009081  |0.000248 |0.000612 |3816     |3512    |3504      |
|flat30-31.cnf        |90       |300    |SAT   |0.0010212  |0.0002419|0.0010632|3820     |3608    |3508      |
|flat30-83.cnf        |90       |300    |SAT   |0.0009296  |0.0002517|0.0007894|3824     |3592    |3628      |
|flat30-45.cnf        |90       |300    |SAT   |0.0008519  |0.0002124|0.000604 |3820     |3496    |3488      |
|uf100-01.cnf         |100      |430    |SAT   |6.4996169  |0.1323839|0.0051964|5700     |3788    |3832      |
|uf100-035.cnf        |100      |430    |SAT   |2.7282803  |0.1106618|0.0014171|5664     |3844    |3852      |
|uf100-0105.cnf       |100      |430    |SAT   |0.8706671  |0.0849393|0.0011785|5240     |3828    |3516      |
|uf100-046.cnf        |100      |430    |SAT   |24.5960195 |0.1242156|0.0039196|6236     |3824    |3948      |
|uuf125-027.cnf       |125      |538    |UNSAT |73.4218055 |1.0494567|0.9109051|6736     |4140    |3768      |
|uuf125-064.cnf       |125      |538    |UNSAT |70.3673839 |0.9673022|0.1664602|6248     |3768    |3776      |
|uuf150-067.cnf       |150      |645    |UNSAT |524.9826428|4.8735956|2.7398417|7936     |4076    |4112      |
|uf150-01.cnf         |150      |645    |SAT   |53.7876964 |0.2357285|0.0397964|8024     |4004    |4072      |
--------------------------------------------------------------------------------------------------------------

### Time Efficiency Comparison
![Time Efficiency](images/performance_time_analysis.png)

### Memory Usage Comparison
![Memory Usage](images/performance_memory_analysis.png)

---

## Full Report

For detailed analysis, benchmarking methodology, and results, refer to the [Project Report (PDF)](DSDA_Project_Report_Group5.pdf).