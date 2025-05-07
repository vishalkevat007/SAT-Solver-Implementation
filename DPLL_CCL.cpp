// DPLL + CCL

#include <bits/stdc++.h>
#include <iomanip>
#include <filesystem>


#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <psapi.h>
long long getMemoryUsage() {
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize / 1024;   // For KB
    } else {
        std::cerr << "Error: GetProcessMemoryInfo failed on Windows." << std::endl;
        return -1;
    }
}
#elif defined(__linux__)
#include <unistd.h>
long long getMemoryUsage() {
    std::ifstream status_file("/proc/" + std::to_string(getpid()) + "/status");
    std::string line;
    while (std::getline(status_file, line)) {
        if (line.find("VmRSS:") == 0) {
            std::stringstream ss(line.substr(6));
            long long memory_kb;
            ss >> memory_kb;
            return memory_kb;
        }
    }
    std::cerr << "Error: Could not read VmRSS from /proc/self/status on Linux." << std::endl;
    return -1;
}
#else
long long getMemoryUsage() {
    std::cerr << "Error: Memory usage measurement is not implemented for this platform." << std::endl;
    return -1;
}
#endif

using namespace std;
using namespace std::chrono;

//--------------------------------------------------------------
// Data Structures
//--------------------------------------------------------------

// Holds variable information: current truth value and clause indices for each polarity.
struct VariableInfo {
    int value; // -1 = unassigned; 1 = true; 0 = false.
    vector<int> positive_clauses;
    vector<int> negative_clauses;
    // VSIDS counters removed.
    VariableInfo() : value(-1) {}
};

// Holds conflict-related information. In case a conflict is detected,
// the "reason" field stores the conflict clause (learned clause).
struct AssignmentStatus {
    bool conflict;
    unordered_set<int> reason;
    AssignmentStatus() : conflict(false) {}
};

//--------------------------------------------------------------
// DPLLSolver_DS Class: Basic DPLL with Conflict Clause Learning Only
//--------------------------------------------------------------
class DPLLSolver_DS {
public:
    // Data members:
    vector<unordered_set<int>> cnf;         // Dynamic CNF, updated during search
    vector<unordered_set<int>> original_cnf;  // Original CNF (with learned clauses added)
    vector<VariableInfo> variables;           // Variables, index 0 unused (variables numbered from 1)
    vector<int> assignments_stack;            // Stack of current assignments (for backtracking)
    unordered_set<int> unsatisfied_clauses;   // Indices of clauses not yet satisfied
    AssignmentStatus last_assignment_status;  // Stores conflict information

    // Learning parameters for conflict clause learning.
    int max_learned_clause_len;
    int max_learned_clauses;
    int learned_clauses_count;
    int decision_count;

    // Constructor: initializes the solver from the parsed CNF.
    DPLLSolver_DS(const vector<unordered_set<int>>& cnf_clauses,
                  int learned_clause_limit_percentage = 25,
                  int max_learned_clause_len_param = 5)
        : max_learned_clause_len(max_learned_clause_len_param + 1),
          learned_clauses_count(0),
          decision_count(0)
    {
        // Copy input clauses into both dynamic (cnf) and original CNF representations.
        for (const auto &clause : cnf_clauses) {
            cnf.push_back(clause);
            original_cnf.push_back(clause);
        }
        // Initially, mark all clauses as unsatisfied.
        for (int i = 0; i < (int)cnf.size(); i++) {
            unsatisfied_clauses.insert(i);
        }
        // Determine the maximum variable number.
        int maxVar = 0;
        for (const auto &clause : cnf_clauses)
            for (int lit : clause)
                maxVar = max(maxVar, abs(lit));
        // Resize the variables vector (index 0 unused).
        variables.resize(maxVar + 1);
        // Populate each variable's clause lists.
        for (size_t i = 0; i < cnf_clauses.size(); i++) {
            for (int lit : cnf_clauses[i]) {
                int var = abs(lit);
                if (lit > 0)
                    variables[var].positive_clauses.push_back(i);
                else
                    variables[var].negative_clauses.push_back(i);
            }
        }
        // Set the maximum number of learned clauses allowed.
        max_learned_clauses = (int)(cnf_clauses.size() * (learned_clause_limit_percentage / 100.0));
    }

    // Public method to solve the SAT instance.
    bool solve() {
        assignments_stack.clear();
        return dpll();
    }

    // Prints the final assignment in sorted order; unassigned variables default to 0.
    void print_assignments() {
        cout << "ASSIGNMENT: ";
        for (size_t i = 1; i < variables.size(); i++) {
            int value = (variables[i].value == -1 ? 0 : variables[i].value);
            cout << i << "=" << value << " ";
        }
        cout << "\n";
    }

    // Optionally returns assignments as a map.
    unordered_map<int, int> get_assignments() {
        unordered_map<int, int> assignments;
        for (size_t i = 1; i < variables.size(); i++) {
            if (variables[i].value != -1)
                assignments[i] = (variables[i].value == 1 ? 1 : 0);
        }
        return assignments;
    }

private:
    //--------------------------------------------------------------
    // Core DPLL Functions with Conflict Clause Learning (No VSIDS)
    //--------------------------------------------------------------

    // The main recursive DPLL algorithm.
    bool dpll() {
        decision_count++;

        // --- Unit Propagation ---
        vector<int> unit_clauses;
        for (int idx : unsatisfied_clauses) {
            if (cnf[idx].size() == 1)
                unit_clauses.push_back(idx);
        }
        while (!unit_clauses.empty()) {
            int idx = unit_clauses.back();
            unit_clauses.pop_back();
            if (cnf[idx].empty())
                continue;
            int unit = *(cnf[idx].begin());
            if (!assign(unit)) {
                // On conflict, if conflict clause is small enough, learn it.
                if (last_assignment_status.conflict && last_assignment_status.reason.size() < (size_t)max_learned_clause_len)
                    add_learned_clause(last_assignment_status.reason);
                unassign(unit);
                return false;
            }
            assignments_stack.push_back(unit);
            unsatisfied_clauses.erase(idx);
            // Recompute unit clauses after propagation.
            unit_clauses.clear();
            for (int j : unsatisfied_clauses)
                if (cnf[j].size() == 1)
                    unit_clauses.push_back(j);
        }
        if (unsatisfied_clauses.empty())
            return true;

        // --- Pure Literal Elimination ---
        while (true) {
            unordered_set<int> pure_literals = find_pure_literals();
            if (pure_literals.empty())
                break;
            for (int lit : pure_literals) {
                if (!assign(lit)) {
                    unassign(lit);
                    return false;
                }
                assignments_stack.push_back(lit);
            }
        }
        if (unsatisfied_clauses.empty())
            return true;

        // --- Branching ---
        // Remove VSIDS entirely; select the branch literal from the first unsatisfied clause.
        int idx = *unsatisfied_clauses.begin();
        int branch_lit = *(cnf[idx].begin());
        if (branch_lit == 0)
            return unsatisfied_clauses.empty();

        // Try branching with branch_lit and its negation.
        for (int val : {branch_lit, -branch_lit}) {
            if (assign(val)) {
                assignments_stack.push_back(val);
                size_t last_stack_length = assignments_stack.size();
                if (dpll())
                    return true;
                // Backtrack: unassign assignments made in this branch.
                while (assignments_stack.size() >= last_stack_length) {
                    int popped = assignments_stack.back();
                    assignments_stack.pop_back();
                    unassign(popped);
                }
                unassign(val);
            } else {
                if (last_assignment_status.conflict && last_assignment_status.reason.size() < (size_t)max_learned_clause_len) {
                    add_learned_clause(last_assignment_status.reason);
                    unassign(val);
                    return false;
                }
                unassign(val);
            }
        }
        return false;
    }

    //--------------------------------------------------------------
    // Helper Functions for DPLL: Propagation, Branching, and Learning
    //--------------------------------------------------------------

    // Finds pure literals: variables that appear only in one polarity in unsatisfied clauses.
    unordered_set<int> find_pure_literals() {
        unordered_map<int, int> candidate_literals;
        for (int idx : unsatisfied_clauses) {
            for (int lit : cnf[idx])
                candidate_literals[lit]++;
        }
        unordered_set<int> pure;
        for (const auto &p : candidate_literals) {
            int lit = p.first;
            if (candidate_literals.find(-lit) == candidate_literals.end() && variables[abs(lit)].value == -1)
                pure.insert(lit);
        }
        return pure;
    }

    // Branching selection: choose a literal from the clause lists (VSIDS removed).
    // (This function is now integrated directly into dpll() for basic branching.)

    // Attempts to assign a literal and update affected clauses.
    bool assign(int literal) {
        int var = abs(literal);
        // Reset conflict status.
        last_assignment_status.conflict = false;
        last_assignment_status.reason.clear();

        // If already assigned, check for consistency.
        if (variables[var].value != -1) {
            return (variables[var].value == (literal > 0 ? 1 : 0));
        }
        // Set the assignment.
        variables[var].value = (literal > 0 ? 1 : 0);

        int count = 0;
        unordered_set<int> conflict_clause;
        // Process clauses where the literal appears in the satisfied polarity.
        vector<int>& clauseList = (literal > 0 ? variables[var].positive_clauses : variables[var].negative_clauses);
        for (int idx : clauseList) {
            if (unsatisfied_clauses.find(idx) != unsatisfied_clauses.end()) {
                bool all_assigned_false = true;
                for (int lit_in_clause : original_cnf[idx]) {
                    if (lit_in_clause == literal) continue;
                    int assigned = variables[abs(lit_in_clause)].value;
                    if (assigned == -1) { all_assigned_false = false; break; }
                    int required = (lit_in_clause < 0 ? 1 : 0);
                    if (assigned != required) { all_assigned_false = false; break; }
                }
                if (all_assigned_false && count == 0) {
                    count = 1;
                    for (int lit_in_clause : original_cnf[idx]) {
                        int required = (lit_in_clause < 0 ? 1 : 0);
                        if (variables[abs(lit_in_clause)].value == required)
                            conflict_clause.insert(lit_in_clause);
                    }
                }
                unsatisfied_clauses.erase(idx);
            }
        }
        // Process clauses where the literal appears with the opposite polarity.
        vector<int>& clauseList2 = (literal > 0 ? variables[var].negative_clauses : variables[var].positive_clauses);
        for (int idx : clauseList2) {
            if (unsatisfied_clauses.find(idx) != unsatisfied_clauses.end()) {
                if (cnf[idx].size() == 1 && count == 1) {
                    for (int lit_in_clause : original_cnf[idx]) {
                        if (lit_in_clause != -literal)
                            conflict_clause.insert(lit_in_clause);
                    }
                    last_assignment_status.conflict = true;
                    last_assignment_status.reason = conflict_clause;
                    variables[var].value = -1;  // Revert assignment on conflict.
                    return false;
                }
                // Remove the literal that is now false.
                cnf[idx].erase(-literal);
            }
        }
        return true;
    }

    // Reverts the assignment of a literal and restores affected clauses.
    void unassign(int literal) {
        int var = abs(literal);
        variables[var].value = -1;
        vector<int>& negList = (literal > 0 ? variables[var].negative_clauses : variables[var].positive_clauses);
        vector<int>& posList = (literal > 0 ? variables[var].positive_clauses : variables[var].negative_clauses);
        for (int idx : negList)
            cnf[idx].insert(-literal);
        for (int idx : posList)
            if (!is_clause_satisfied(idx))
                unsatisfied_clauses.insert(idx);
    }

    // Checks if a clause is satisfied under current assignments.
    bool is_clause_satisfied(int clause_index) {
        for (int lit : cnf[clause_index]) {
            int var = abs(lit);
            if (variables[var].value != -1) {
                int required = (lit > 0 ? 1 : 0);
                if (variables[var].value == required)
                    return true;
            }
        }
        return false;
    }

    // Adds a conflict-learned clause to the CNF.
    void add_learned_clause(const unordered_set<int>& learned_clause) {
        if (learned_clause.empty())
            return;
        if (learned_clauses_count >= max_learned_clauses)
            return;
        unordered_set<int> new_clause;
        for (int lit : learned_clause) {
            int var = abs(lit);
            if (variables[var].value == -1 || variables[var].value == (lit > 0 ? 1 : 0))
                new_clause.insert(lit);
        }
        // Copy the original learned clause.
        unordered_set<int> new_clause_original = learned_clause;
        original_cnf.push_back(new_clause_original);
        cnf.push_back(new_clause);
        int learned_clause_index = cnf.size() - 1;
        unsatisfied_clauses.insert(learned_clause_index);
        // Update variable clause lists for the newly learned clause.
        for (int lit : new_clause_original) {
            int var = abs(lit);
            if (lit > 0)
                variables[var].positive_clauses.push_back(learned_clause_index);
            else
                variables[var].negative_clauses.push_back(learned_clause_index);
        }
        learned_clauses_count++;
        // In this basic version, VSIDS boosting is omitted.
    }

    // For basic conflict clause learning only, we remove VSIDS decay functions.
    void decay_counters() { /* Not used */ }
    void boost_conflict_literals(const unordered_set<int>& conflict_clause) { /* Not used */ }
};

//--------------------------------------------------------------
// CNF File Loader (DIMACS Format)
//--------------------------------------------------------------
// Reads a CNF file and returns a vector of clauses (each clause is an unordered_set<int>).
vector<unordered_set<int>> load_cnf(const string &filename) {
    ifstream infile(filename);
    string line;
    vector<unordered_set<int>> clauses;
    while (getline(infile, line)) {
        if (line.empty() || line[0] == 'c' || line[0] == 'p' ||
            line[0] == '%' || line[0] == '0')
            continue;
        istringstream iss(line);
        int lit;
        vector<int> lits;
        while (iss >> lit)
            lits.push_back(lit);
        // Omit the trailing 0.
        unordered_set<int> clause;
        for (size_t i = 0; i < lits.size() - 1; i++) {
            clause.insert(lits[i]);
        }
        clauses.push_back(clause);
    }
    if (clauses.empty())
    {
        throw runtime_error("Error: No valid clauses found in CNF file.");
    }
    return clauses;
}

//--------------------------------------------------------------
// Output Function: Print Final Assignment
//--------------------------------------------------------------
void print_assignment(const vector<int> &assignment, int numLiterals) {
    vector<int> solution(numLiterals + 1, -1); // -1 = unassigned (treated as false)
    for (int lit : assignment) {
        int var = abs(lit);
        solution[var] = (lit > 0 ? 1 : 0);
    }
    cout << "ASSIGNMENT: ";
    for (int var = 1; var <= numLiterals; var++) {
        cout << var << "=" << (solution[var] == 1 ? 1 : 0) << " ";
    }
    cout << "\n";
}

//--------------------------------------------------------------
// Main Function
//--------------------------------------------------------------
int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: ./mySAT2 <cnf_file>\n";
        return 1;
    }

    try
    {
        string file_path = argv[1];
        if (file_path.size() < 4 || file_path.substr(file_path.size() - 4) != ".cnf" || !std::filesystem::exists(file_path))
        {
            cerr << "Error: Input file must be a valid .cnf file and must exist.\n";
            return 1;
        }
        
        vector<unordered_set<int>> clauses = load_cnf(file_path);
        DPLLSolver_DS solver(clauses);

        auto start_time = high_resolution_clock::now();
        bool result = solver.solve();
        auto end_time = high_resolution_clock::now();
        chrono::duration<double> time_taken = end_time - start_time;
        long long memory_used = getMemoryUsage();

        cout << "[DPLL + CCL]\nRESULT: " << (result ? "SAT" : "UNSAT") << "\n";
        if (result)
            solver.print_assignments();
        std::cout << std::fixed << std::setprecision(7);
        cout << "Time taken: " << time_taken.count() << " seconds\n";
        // Reset formatting to default for memory output
        std::cout.unsetf(std::ios_base::floatfield);
        cout << "Memory used: " << memory_used << " KB\n";
    
    }
    catch (const exception& e)
    {
        cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}
