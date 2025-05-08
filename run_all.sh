#!/bin/bash

# SAT Solver executable (make this a variable)
sat_solver_name="DPLL_CDCL_VSIDS" # Changed to just the name
sat_solver_exe="./$sat_solver_name" # Prepend "./"
sat_solver_cpp="$sat_solver_name.cpp" # Added source file variable

# Compile the SAT solver
echo "Compiling $sat_solver_cpp..."
g++ -std=c++17 -O2 "$sat_solver_cpp" -o "$sat_solver_exe"

if [ $? -ne 0 ]; then
    echo "Compilation failed. Exiting."
    exit 1
fi
echo "Compilation successful."
echo ""

# Define the subfolder containing CNF files
folder="benchmark_files"

# Check if the folder exists
if [ ! -d "$folder" ]; then
    echo "Folder \"$folder\" not found!"
    exit 1
fi

# Check if any .cnf files exist in the folder
shopt -s nullglob      # Ensure that the glob expands to an empty array if no files match
cnf_files=("$folder"/*.cnf)
if [ ${#cnf_files[@]} -eq 0 ]; then
    echo "No .cnf files found in \"$folder\"."
    exit 1
fi

# Loop through each .cnf file in the folder
for file in "$folder"/*.cnf; do
    echo "----------------------------------------"
    echo "Processing file: $file"
    "$sat_solver_exe" "$file"
    echo ""
done

# Pause at the end (simulating a pause similar to Windows' pause)
read -p "Press any key to continue... " -n1 -s
echo
