import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.ticker import MultipleLocator, FuncFormatter

# Set global fonts
plt.rcParams['font.family'] = ['Times New Roman'] # Font family
plt.rcParams['font.size'] = 14           # Base font size
plt.rcParams['axes.titlesize'] = 20      # Title
plt.rcParams['axes.labelsize'] = 16      # X/Y axis labels
plt.rcParams['xtick.labelsize'] = 12     # X tick labels
plt.rcParams['ytick.labelsize'] = 12     # Y tick labels
plt.rcParams['legend.fontsize'] = 14     # Legend text

# 1. Read the data in original order
df = pd.read_csv('solver_comparison_tabulated.csv')

# 2. Compute per‐row normalized times (seconds)
df['norm_cdcl_time']   = df['CDCL(s)']   / df['DPLL(s)']
df['norm_vsids_time'] = df['VSIDS(s)'] / df['DPLL(s)']

# 3. Compute per‐row normalized memory usage (KB)
df['norm_cdcl_mem']    = df['CDCL(KB)']   / df['DPLL(KB)']
df['norm_vsids_mem']  = df['VSIDS(KB)'] / df['DPLL(KB)']

# Optional: inspect the new columns
print(df[['Variables',
          'DPLL(s)',   'CDCL(s)',   'VSIDS(s)',
          'norm_cdcl_time',   'norm_vsids_time',
          'DPLL(KB)',  'CDCL(KB)',  'VSIDS(KB)',
          'norm_cdcl_mem',    'norm_vsids_mem']])

# 4. X positions: one slot per row, in file order
x = range(len(df))

# ─────────────────────────────────────────────
# Part A: Time‐efficiency plot
# ─────────────────────────────────────────────
fig, ax = plt.subplots(figsize=(12,6))

# DPLL baseline at 1.0
ax.axhline(1.0, color='green', linestyle='-', linewidth=2, label='DPLL')

# Normalized time curves
ax.plot(x, df['norm_cdcl_time'], linestyle='-',
        label='DPLL + Conflict-Driven Clause Learning')
ax.plot(x, df['norm_vsids_time'], linestyle='-',
        label='DPLL + Conflict-Driven Clause Learning + VSIDS')

# X‐ticks and labels
ax.set_xticks(x)
ax.set_xticklabels(df['Variables'], rotation=45)

# Y‐ticks every 0.25
ax.yaxis.set_major_locator(MultipleLocator(0.25))

# Format tick labels: only show integers (e.g., 0, 1, 2, …)
# ax.yaxis.set_major_formatter(FuncFormatter(lambda val, pos: f'{int(val)}' if val.is_integer() else ''))

# Labels, title, legend, grid
ax.set_xlabel('Number of Input Variables', fontweight='bold')
ax.set_ylabel('Normalized Time (Relative to DPLL)', fontweight='bold')
ax.set_title("SAT Solver Algorithm's Time Efficiency Across Variable Size Variation")
ax.legend()
ax.legend(loc='upper center', bbox_to_anchor=(0.5, -0.2), ncol=3)
ax.grid(axis='x', linestyle='--')
ax.grid(axis='y', linestyle='--')

plt.tight_layout()
plt.savefig('./images/performance_time_analysis.png', dpi=300)
plt.savefig('./images/performance_time_analysis.pdf', bbox_inches='tight')
plt.close()
print("Saved time‐efficiency plot as performance_time_analysis")


# ─────────────────────────────────────────────
# Part B: Memory‐usage plot
# ─────────────────────────────────────────────
fig, ax = plt.subplots(figsize=(12,6))

# DPLL baseline at 1.0
ax.axhline(1.0, color='green', linestyle='-', linewidth=2, label='DPLL')

# Normalized memory curves
ax.plot(x, df['norm_cdcl_mem'], linestyle='-',
        label='DPLL + Conflict-Driven Clause Learning')
ax.plot(x, df['norm_vsids_mem'], linestyle='-',
        label='DPLL + Conflict-Driven Clause Learning + VSIDS')

# X‐ticks and labels
ax.set_xticks(x)
ax.set_xticklabels(df['Variables'], rotation=45)

# Y‐ticks every 0.25
ax.yaxis.set_major_locator(MultipleLocator(0.25))

# Labels, title, legend, grid
ax.set_xlabel('Number of Input Variables', fontweight='bold')
ax.set_ylabel('Normalized Memory Usage (Relative to DPLL)', fontweight='bold')
ax.set_title("SAT Solver Algorithm's Memory Usage Across Variable Size Variation")
ax.legend()
ax.legend(loc='upper center', bbox_to_anchor=(0.5, -0.2), ncol=3)
ax.grid(axis='x', linestyle='--')
ax.grid(axis='y', linestyle='--')

plt.tight_layout()
plt.savefig('./images/performance_memory_analysis.png', dpi=300)
plt.savefig('./images/performance_memory_analysis.pdf', bbox_inches='tight')
plt.close()
print("Saved memory‐usage plot as performance_memory_analysis")
