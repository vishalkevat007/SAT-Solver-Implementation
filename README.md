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
|uf20-0941.cnf        |20       |91     |SAT   |0.000456  |0.000142|6.1e-05 |3516    |2072   |2076     |
|uf20-0771.cnf        |20       |91     |SAT   |0.000209  |0.000226|0.000123|3624    |2064   |2024     |
|uf20-0114.cnf        |20       |91     |SAT   |0.000582  |0.000129|5.5e-05 |3584    |1996   |2084     |
|uf20-0681.cnf        |20       |91     |SAT   |0.000106  |0.000171|4.5e-05 |2080    |2084   |2080     |
|uf20-0518.cnf        |20       |91     |SAT   |0.000192  |0.000141|4.3e-05 |2024    |2084   |2024     |
|uf20-0159.cnf        |20       |91     |SAT   |0.001028  |0.000191|9.8e-05 |3632    |2020   |2080     |
|aim-50-2_0-yes1-4.cnf|50       |100    |SAT   |0.283408  |0.000334|0.000379|4000    |2080   |2016     |
|aim-50-3_4-yes1-4.cnf|50       |170    |SAT   |0.015301  |0.000282|0.000133|3792    |3584   |3504     |
|uuf50-0152.cnf       |50       |218    |UNSAT |0.117977  |0.003189|0.003118|4092    |3572   |3584     |
|uuf50-063.cnf        |50       |218    |UNSAT |0.035431  |0.00396 |0.001683|4072    |3580   |3636     |
|uuf50-057.cnf        |50       |218    |UNSAT |0.011594  |0.004102|0.001887|3800    |3464   |3544     |
|uf75-011.cnf         |75       |325    |SAT   |0.335553  |0.028611|0.003167|4872    |3492   |3500     |
|uf75-01.cnf          |75       |325    |SAT   |0.005258  |0.001838|0.000472|4392    |3580   |3548     |
|uf75-012.cnf         |75       |325    |SAT   |0.461544  |0.003589|0.02224 |4964    |3748   |3724     |
|uuf75-013.cnf        |75       |325    |UNSAT |0.81952   |0.04044 |0.01833 |4828    |3556   |3732     |
|flat30-100.cnf       |90       |300    |SAT   |0.000877  |0.00025 |0.000593|3864    |3584   |3492     |
|flat30-31.cnf        |90       |300    |SAT   |0.000921  |0.000264|0.000481|3892    |3516   |3500     |
|flat30-83.cnf        |90       |300    |SAT   |0.000873  |0.000232|0.000769|3864    |3460   |3556     |
|flat30-45.cnf        |90       |300    |SAT   |0.000899  |0.000221|0.00041 |3808    |3484   |3500     |
|uf100-01.cnf         |100      |430    |SAT   |6.586744  |0.133104|0.005111|5728    |3868   |3876     |
|uf100-035.cnf        |100      |430    |SAT   |2.640361  |0.107553|0.001444|5952    |3836   |3968     |
|uf100-0105.cnf       |100      |430    |SAT   |0.872136  |0.084558|0.001215|5296    |3804   |3876     |
|uf100-046.cnf        |100      |430    |SAT   |25.86565  |0.125699|0.003917|6060    |3836   |3776     |
|uuf125-027.cnf       |125      |538    |UNSAT |73.29032  |1.046961|0.907709|6748    |4012   |3756     |
|uuf125-064.cnf       |125      |538    |UNSAT |71.059145 |-1.0    |0.16219 |6456    |3820   |3912     |
|uuf150-067.cnf       |150      |645    |UNSAT |525.621352|4.786906|2.667552|8120    |4144   |4088     |
|uf150-01.cnf         |150      |645    |SAT   |52.501979 |0.244321|0.039902|8068    |4168   |4184     |

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