import matplotlib.pyplot as plt
import numpy as np

# X-axis: Number of queries
Q_values = [10, 100, 1000, 10000, 100000, 10_000_000, 100_000_000]

# Runtime data in milliseconds
sol1_raw = [10, 85, 826, 8124, 83503, None, None]  # Predictive last value
sol2 = [0, 0, 34, 128, 1282, 12571, 126368]
sol3 = [4, 5, 7, 22, 174, 1415, 13904]
official = [3, 3, 4, 9, 59, 566, 5416]

# Clean sol1 line (excluding last None)
Q_sol1 = Q_values[:len(sol1_raw) - 1]
sol1_clean = sol1_raw[:len(sol1_raw) - 1]

# Last point to be shown as a dot (no line to it)
Q_sol1_last = Q_values[-1]
sol1_last = sol1_raw[-1]

plt.figure(figsize=(10, 6))

# Plot Solution I
plt.plot(Q_sol1, sol1_clean, marker='o', label='Solution I (Naive)', color='tab:blue')
if sol1_last is not None:
    plt.scatter(Q_sol1_last, sol1_last, marker='o', color='tab:blue')  # Last point as dot

# Plot others
plt.plot(Q_values, sol2, marker='s', label='Solution II (Prefix Max)', color='tab:orange')
plt.plot(Q_values, sol3, marker='^', label='Solution III (Optimized)', color='tab:green')
plt.plot(Q_values, official, marker='x', label='Official', color='tab:red')

plt.xscale('log')

plt.xlabel('Number of Queries (Q)')
plt.ylabel('Runtime (ms)')
plt.title('Runtime Comparison of Solutions vs Number of Queries')
plt.grid(True, which='both', linestyle='--', linewidth=0.5)
plt.legend()
plt.tight_layout()
plt.savefig("runtime_comparison.png")
plt.show()
