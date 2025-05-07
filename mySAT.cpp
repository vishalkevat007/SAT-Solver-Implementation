// DPLL + CCL + VSIDS

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <cstdlib>
#include <algorithm>
// #include <chrono>
#include <iomanip>
#include <filesystem>

using namespace std;

// Structure to hold variable information.
struct VariableInfo {
    // value: -1 means unassigned; 1 means True; 0 means False.
    int value;
    vector<int> positive_clauses;
    vector<int> negative_clauses;
    double pos_count;
    double neg_count;
    VariableInfo() : value(-1), pos_count(0.0), neg_count(0.0) {}
};

// Structure for assignment status (used for conflict analysis).
struct AssignmentStatus {
    bool conflict;
    unordered_set<int> reason;
    AssignmentStatus() : conflict(false) {}
};

class DPLLSolver_DS {
public:
    // Data members
vector<unordered_set<int>> cnf;                  // Dynamic CNF (clauses updated during search)
    vector<unordered_set<int>> original_cnf;     // Original CNF (learned clauses added here)
    vector<VariableInfo> variables;              // Indexed by variable number (1-indexed; index 0 unused)
    vector<int> assignments_stack;               // Stack of current assignments (literals)
    unordered_set<int> unsatisfied_clauses;      // Set of clause indices that are not yet satisfied

    AssignmentStatus last_assignment_status;

    int max_learned_clause_len;
    int max_learned_clauses;
    int learned_clauses_count;
    double decay_factor;
    double conflict_weight;
    int decay_interval;
    int decision_count;

    // Constructor: cnf_clauses is a vector of clauses (each clause is a set of literals)
    DPLLSolver_DS(const vector<unordered_set<int>>& cnf_clauses,
                  int learned_clause_limit_percentage = 25,
                  int max_learned_clause_len_param = 5)
        : max_learned_clause_len(max_learned_clause_len_param + 1),
          learned_clauses_count(0),
          decay_factor(0.95),
          conflict_weight(0.2),
          decay_interval(10),
          decision_count(0)
    {
        // Copy input clauses
        for (const auto& clause : cnf_clauses) {
            cnf.push_back(clause);
            original_cnf.push_back(clause);
        }
        // Initially, all clauses are unsatisfied.
        for (int i = 0; i < (int)cnf.size(); i++) {
            unsatisfied_clauses.insert(i);
        }
        // Determine maximum variable number.
        int maxVar = 0;
        for (const auto& clause : cnf_clauses) {
            for (int lit : clause) {
                maxVar = max(maxVar, abs(lit));
            }
        }
        // Resize variables vector (index 0 unused).
        variables.resize(maxVar + 1);
        // Populate clause lists for each variable.
        for (size_t i = 0; i < cnf_clauses.size(); i++) {
            for (int lit : cnf_clauses[i]) {
                int var = abs(lit);
                if (lit > 0)
                    variables[var].positive_clauses.push_back(i);
                else
                    variables[var].negative_clauses.push_back(i);
            }
        }
        // Maximum learned clauses allowed.
        max_learned_clauses = (int)(cnf_clauses.size() * (learned_clause_limit_percentage / 100.0));
    }

    // Public solve method.
    bool solve() {
        assignments_stack.clear();
        return dpll();
    }

    // Print assignments in sorted order.
    void print_assignments() {
        cout << "ASSIGNMENT: ";
        // Print all variables from 1 to variables.size()-1.
        for (size_t i = 1; i < variables.size(); i++) {
            // If a variable was never assigned, default its value to 0.
            int value = (variables[i].value == -1 ? 0 : variables[i].value);
            cout << i << "=" << value << " ";
            }
        cout << "\n";
    }

    // Get current assignments as a map.
    unordered_map<int, int> get_assignments() {
        unordered_map<int, int> assignments;
        for (size_t i = 1; i < variables.size(); i++) {
            if (variables[i].value != -1)
                assignments[i] = (variables[i].value == 1 ? 1 : 0);
        }
        return assignments;
    }

private:
    // The main recursive DPLL algorithm.
    bool dpll() {
        decision_count++;
        if (decision_count % decay_interval == 0) {
            decision_count = 0;
            decay_counters();
        }

        // Unit Clause Propagation
        vector<int> unit_clauses;
        for (int idx : unsatisfied_clauses) {
            if (cnf[idx].size() == 1)
                unit_clauses.push_back(idx);
        }
        while (!unit_clauses.empty()) {
            int idx = unit_clauses.back();
            unit_clauses.pop_back();
            if (cnf[idx].empty()) continue;
            int unit = *(cnf[idx].begin());
            if (!assign(unit)) {
                if (last_assignment_status.conflict && last_assignment_status.reason.size() < (size_t)max_learned_clause_len)
                    add_learned_clause(last_assignment_status.reason);
                unassign(unit);
                return false;
            }
            assignments_stack.push_back(unit);
            unsatisfied_clauses.erase(idx);
            // Recompute unit clauses after propagation.
            unit_clauses.clear();
            for (int j : unsatisfied_clauses) {
                if (cnf[j].size() == 1)
                    unit_clauses.push_back(j);
            }
        }
        if (unsatisfied_clauses.empty())
            return true;

        // Pure Literal Elimination
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

        // Branching: choose a literal based on VSIDS frequency.
        int branch_lit = vsids_frequent_literal();
        if (branch_lit == 0)
            return unsatisfied_clauses.empty();

        // Try assigning branch_lit and then its negation with backtracking.
        for (int val : {branch_lit, -branch_lit}) {
            if (assign(val)) {
                assignments_stack.push_back(val);
                size_t last_stack_length = assignments_stack.size();
                if (dpll())
                    return true;
                // Backtrack: unassign all assignments added in the recursive call.
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

    // Find pure literals that appear with only one polarity.
    unordered_set<int> find_pure_literals() {
        unordered_map<int, int> candidate_literals;
        for (int idx : unsatisfied_clauses) {
            for (int lit : cnf[idx]) {
                candidate_literals[lit]++;
            }
        }
        unordered_set<int> pure;
        for (const auto& p : candidate_literals) {
            int lit = p.first;
            if (candidate_literals.find(-lit) == candidate_literals.end() && variables[abs(lit)].value == -1)
                pure.insert(lit);
        }
        return pure;
    }

    // VSIDS: select the most frequently occurring unassigned literal.
    int vsids_frequent_literal() {
        if (unsatisfied_clauses.empty())
            return 0;
        unordered_map<int, double> literal_counts;
        for (int idx : unsatisfied_clauses) {
            for (int lit : cnf[idx]) {
                if (variables[abs(lit)].value == -1) {
                    double weight = 1.0 + (lit > 0 ? variables[abs(lit)].pos_count : variables[abs(lit)].neg_count);
                    literal_counts[lit] += weight;
                }
            }
        }
        if (literal_counts.empty())
            return 0;
        int best_literal = 0;
        double best_count = -1;
        for (const auto& p : literal_counts) {
            if (p.second > best_count) {
                best_count = p.second;
                best_literal = p.first;
            }
        }
        return best_literal;
    }

    // Try to assign a literal and update affected clauses.
    bool assign(int literal) {
        int var = abs(literal);
        // Reset conflict status.
        last_assignment_status.conflict = false;
        last_assignment_status.reason.clear();

        // If already assigned, check for consistency.
        if (variables[var].value != -1) {
            if (variables[var].value == (literal > 0 ? 1 : 0))
                return true;
            else
                return false;
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
                for (int lit : original_cnf[idx]) {
                    if (lit == literal) continue;
                    int assigned = variables[abs(lit)].value;
                    if (assigned == -1) { all_assigned_false = false; break; }
                    int required = (lit < 0 ? 1 : 0);
                    if (assigned != required) { all_assigned_false = false; break; }
                }
                if (all_assigned_false && count == 0) {
                    count = 1;
                    for (int lit : original_cnf[idx]) {
                        int required = (lit < 0 ? 1 : 0);
                        if (variables[abs(lit)].value == required)
                            conflict_clause.insert(lit);
                    }
                }
                unsatisfied_clauses.erase(idx);
            }
        }
        // Process clauses where the literal appears in the opposite polarity.
        vector<int>& clauseList2 = (literal > 0 ? variables[var].negative_clauses : variables[var].positive_clauses);
        for (int idx : clauseList2) {
            if (unsatisfied_clauses.find(idx) != unsatisfied_clauses.end()) {
                if (cnf[idx].size() == 1 && count == 1) {
                    for (int lit : original_cnf[idx]) {
                        if (lit != -literal)
                            conflict_clause.insert(lit);
                    }
                    last_assignment_status.conflict = true;
                    last_assignment_status.reason = conflict_clause;
                    variables[var].value = -1;  // revert assignment
                    return false;
                }
                // Remove the literal that is now false.
                cnf[idx].erase(-literal);
            }
        }
        return true;
    }

    // Revert the assignment of a literal and restore the affected clauses.
    void unassign(int literal) {
        int var = abs(literal);
        variables[var].value = -1;
        vector<int>& clauses_with_negative = (literal > 0 ? variables[var].negative_clauses : variables[var].positive_clauses);
        vector<int>& clauses_with_chosen = (literal > 0 ? variables[var].positive_clauses : variables[var].negative_clauses);
        for (int idx : clauses_with_negative) {
            cnf[idx].insert(-literal);
        }
        for (int idx : clauses_with_chosen) {
            if (!is_clause_satisfied(idx))
                unsatisfied_clauses.insert(idx);
        }
    }

    // Check if a clause is satisfied.
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

    // Add a learned clause derived from a conflict.
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
        unordered_set<int> new_clause_original = learned_clause; // copy original learned clause
        original_cnf.push_back(new_clause_original);
        cnf.push_back(new_clause);
        int learned_clause_index = cnf.size() - 1;
        unsatisfied_clauses.insert(learned_clause_index);
        // Update variable clause lists.
        for (int lit : new_clause_original) {
            int var = abs(lit);
            if (lit > 0)
                variables[var].positive_clauses.push_back(learned_clause_index);
            else
                variables[var].negative_clauses.push_back(learned_clause_index);
        }
        learned_clauses_count++;
        boost_conflict_literals(learned_clause);
    }

    // Decay the conflict counters.
    void decay_counters() {
        for (auto& var : variables) {
            var.pos_count *= decay_factor;
            var.neg_count *= decay_factor;
        }
    }

    // Boost the counters for conflict literals.
    void boost_conflict_literals(const unordered_set<int>& conflict_clause) {
        for (int lit : conflict_clause) {
            int var = abs(lit);
            if (lit > 0)
                variables[var].pos_count += conflict_weight;
            else
                variables[var].neg_count += conflict_weight;
        }
        decay_counters();
    }
};

// Helper function to load a CNF file and parse the clauses.
vector<unordered_set<int>> load_cnf(const string& filename) {
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
        while (iss >> lit) {
            lits.push_back(lit);
        }
        // The last element is typically 0 and omitted.
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

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: ./mySAT <cnf_file>\n";
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
        bool result = solver.solve();
        cout << "RESULT: " << (result ? "SAT" : "UNSAT") << "\n";
        if (result)
            solver.print_assignments();
    }
    catch (const exception& e)
    {
        cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}
