import subprocess
import os
import re
import pandas as pd
 
# Binaries and their labels
solvers = [
    ("./DPLL", "DPLL"),
    ("./DPLL_CCL", "CCL"),
    ("./DPLL_CCL_VSIDS", "VSIDS")
]
 
# Folder and list of CNF files to test
benchmark_folder = "benchmark_files"
cnf_files = [f for f in os.listdir(benchmark_folder) if f.endswith(".cnf")]
 
results = []
 
def extract_vars_clauses(file_path):
    with open(file_path, 'r') as f:
        for line in f:
            if line.startswith('p cnf'):
                _, _, vars_str, clauses_str = line.strip().split()
                return int(vars_str), int(clauses_str)
    return -1, -1  # Default if not found
 
# Run each solver on each CNF file
for cnf_file in cnf_files:
    cnf_path = os.path.join(benchmark_folder, cnf_file)
    variables, clauses = extract_vars_clauses(cnf_path)
 
    row = {
        "Benchmark": cnf_file,
        "Variables": variables,
        "Clauses": clauses,
        "Result": ""
    }
 
    for exe, label in solvers:
        try:
            print(f"Running {exe} on {cnf_file}")
            output = subprocess.check_output(
                [exe, cnf_path],
                stderr=subprocess.STDOUT,
                text=True,
                timeout=700
            )
 
            result_match = re.search(r"RESULT:\s+(SAT|UNSAT)", output)
            time_match = re.search(r"Time taken:\s+([\d.]+)", output)
            memory_match = re.search(r"Memory used:\s+([\d.]+)", output)
 
            result = result_match.group(1) if result_match else "N/A"
            time = float(time_match.group(1)) if time_match else -1
            memory_KB = memory_match.group(1) if memory_match else -1
 
            if label == "VSIDS":
                row["Result"] = result
 
            row[f"Time ({label})"] = round(time, 7)
            row[f"Memory ({label})"] = memory_KB
            # row[f"Memory ({label})"] = round(memory_KB, 2)
 
        except subprocess.TimeoutExpired:
            row[f"Time ({label})"] = "TIMEOUT"
            row[f"Memory ({label})"] = "TIMEOUT"
            row["Result"] = "TIMEOUT"
        except Exception as e:
            row[f"Time ({label})"] = f"ERROR"
            row[f"Memory ({label})"] = f"ERROR"
            row["Result"] = "ERROR"
 
    results.append(row)
 
# Display table
df = pd.DataFrame(results)
# Sort by number of variables and clauses
df = df.sort_values(by=["Variables", "Clauses"])
 
# Rename columns for visual match with figure
df = df.rename(columns={
    "Time (DPLL)": "DPLL(s)",
    "Time (CCL)": "CCL(s)",
    "Time (VSIDS)": "VSIDS(s)",
    "Memory (DPLL)": "DPLL(KB)",
    "Memory (CCL)": "CCL(KB)",
    "Memory (VSIDS)": "VSIDS(KB)"
})
 
# Reorder columns
df = df[["Benchmark", "Variables", "Clauses", "Result",
         "DPLL(s)", "CCL(s)", "VSIDS(s)",
         "DPLL(KB)", "CCL(KB)", "VSIDS(KB)"]]
 
# Print table
print(df.to_string(index=False))
 
# Optional: save to CSV
df.to_csv("solver_comparison_tabulated.csv", index=False)