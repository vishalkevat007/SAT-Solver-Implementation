// DPLL Only

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <string>
#include <chrono>
#include <unordered_map>
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

bool dpll(vector<unordered_set<int>>& cnf, unordered_map<int, int>& assignments, int maxVar) {
    // Unit propagation
    while (true) {
        bool progress = false;
        for (auto it = cnf.begin(); it != cnf.end(); ) {
            if (it->size() == 0) return false;
            if (it->size() == 1) {
                int unit = *it->begin();
                int var = abs(unit);
                int val = (unit > 0 ? 1 : 0);
                if (assignments.count(var) && assignments[var] != val) return false;
                assignments[var] = val;
                // propagate
                vector<unordered_set<int>>::iterator inner = cnf.begin();
                while (inner != cnf.end()) {
                    if (inner->count(unit)) {
                        inner = cnf.erase(inner);
                    } else if (inner->count(-unit)) {
                        inner->erase(-unit);
                        ++inner;
                    } else {
                        ++inner;
                    }
                }
                progress = true;
                break;
            } else {
                ++it;
            }
        }
        if (!progress) break;
    }

    if (cnf.empty()) return true;

    int chosen_var = 0;
    for (int v = 1; v <= maxVar; ++v) {
        if (assignments.count(v) == 0) {
            chosen_var = v;
            break;
        }
    }
    if (chosen_var == 0) return false;

    for (int val : {1, 0}) {
        unordered_map<int, int> assignments_copy = assignments;
        vector<unordered_set<int>> cnf_copy = cnf;
        assignments_copy[chosen_var] = val;
        // propagate
        vector<unordered_set<int>>::iterator it = cnf_copy.begin();
        while (it != cnf_copy.end()) {
            if (it->count(val ? chosen_var : -chosen_var)) {
                it = cnf_copy.erase(it);
            } else if (it->count(val ? -chosen_var : chosen_var)) {
                it->erase(val ? -chosen_var : chosen_var);
                ++it;
            } else {
                ++it;
            }
        }
        if (dpll(cnf_copy, assignments_copy, maxVar)) {
            assignments = assignments_copy;
            return true;
        }
    }
    return false;
}

vector<unordered_set<int>> load_cnf(const string& filename, int& maxVar) {
    ifstream infile(filename);
    string line;
    vector<unordered_set<int>> clauses;
    maxVar = 0;
    while (getline(infile, line)) {
        if (line.empty() || line[0] == 'c' || line[0] == 'p') continue;
        istringstream iss(line);
        int lit;
        unordered_set<int> clause;
        while (iss >> lit && lit != 0) {
            clause.insert(lit);
            maxVar = max(maxVar, abs(lit));
        }
        if (!clause.empty()) clauses.push_back(clause);
    }
    if (clauses.empty())
    {
        throw runtime_error("Error: No valid clauses found in CNF file.");
    }
    return clauses;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: ./DPLL <cnf_file>\n";
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

        int maxVar;
        auto clauses = load_cnf(file_path, maxVar);
        unordered_map<int, int> assignments;

        auto start = chrono::high_resolution_clock::now();
        bool sat = dpll(clauses, assignments, maxVar);
        auto end = chrono::high_resolution_clock::now();
        long long memory_used = getMemoryUsage();

        cout << "[DPLL Only]\nRESULT: " << (sat ? "SAT" : "UNSAT") << "\n";
        if (sat) {
            cout << "ASSIGNMENT: ";
            for (int i = 1; i <= maxVar; i++) {
                auto it = assignments.find(i);
                if (it != assignments.end()) {
                    // it->second is 1 for true, 0 for false
                    cout << i << "=" << it->second << " ";
                }
            }
            cout << "\n";
        }
        std::cout << std::fixed << std::setprecision(7);
        cout << "Time taken: " << chrono::duration<double>(end - start).count() << " seconds\n";
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