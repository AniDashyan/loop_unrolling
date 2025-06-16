# Loop Unrolling Test

## Overview
This project demonstrates the impact of loop unrolling and compiler optimizations on the performance of a sum of squares calculation over a large array. It compares basic loops, manually unrolled loops (4x and 8x), and a compiler-optimized version using `std::accumulate`.

## Build & Run
```bash
# 1. Clone the repository:
git clone https://github.com/AniDashyan/loop_unrolling.git

# 2. Navigate to the repository:
cd loop_unrolling

# 3. Build the project:
cmake -S . -B build

# 4. Run the project:
cmake --build build --target run_all
```

## Example Output

```bash 
### Running with -O0 optimization
=== Sum of Squares Loop Unrolling Test ===
Array size: 1000000 elements
Iterations: 1000
Results (average time per iteration):
Basic loop:           6.060 ms (result: 38500000)
4x unrolled:          7.575 ms (result: 38500000)
8x unrolled:          8.533 ms (result: 38500000)
Compiler optimized:   4.141 ms (result: 38500000)
Speedup vs basic loop:
4x unrolled:          0.80x
8x unrolled:          0.71x
Compiler optimized:   1.46x

### Running with -O2 optimization
=== Sum of Squares Loop Unrolling Test ===
Array size: 1000000 elements
Iterations: 1000
Results (average time per iteration):
Basic loop:           0.359 ms (result: 38500000)
4x unrolled:          0.212 ms (result: 38500000)
8x unrolled:          0.177 ms (result: 38500000)
Compiler optimized:   0.360 ms (result: 38500000)
Speedup vs basic loop:
4x unrolled:          1.69x
8x unrolled:          2.03x
Compiler optimized:   1.00x

### Running with -O3 optimization
=== Sum of Squares Loop Unrolling Test ===
Array size: 1000000 elements
Iterations: 1000
Results (average time per iteration):
Basic loop:           0.178 ms (result: 38500000)
4x unrolled:          0.212 ms (result: 38500000)
8x unrolled:          0.162 ms (result: 38500000)
Compiler optimized:   0.172 ms (result: 38500000)
Speedup vs basic loop:
4x unrolled:          0.84x
8x unrolled:          1.10x
Compiler optimized:   1.03x
```

## Explanation
This project evaluates the performance implications of loop unrolling and compiler optimizations on a sum of squares computation over a vector of 1,000,000 integers, executed 1,000 times. Loop unrolling reduces the overhead of loop control by processing multiple elements per iteration, potentially improving instruction-level parallelism but increasing code size and cache pressure. The implementations include:
- **Basic loop:** Processes one element per iteration.
- **4x unrolled:** Processes four elements per iteration.
- **8x unrolled:** Processes eight elements per iteration.
- **Compiler-optimized:** Uses `std::accumulate` with a lambda, allowing the compiler to apply its own optimizations.

### Performance Analysis Across Optimization Levels
- **-O0 (No optimization):** The basic loop takes 6.060 ms per iteration, the slowest due to minimal optimization and high loop overhead. The 4x (7.575 ms) and 8x (8.533 ms) unrolled versions are slower, likely due to increased cache misses from larger code size and less efficient memory access patterns. The compiler-optimized version (4.141 ms) outperforms all, suggesting better memory locality or simpler execution paths. Speedup relative to the basic loop is 0.80x (4x), 0.71x (8x), and 1.46x (compiler-optimized).
- **-O2 (Moderate optimization):** The basic loop improves to 0.359 ms, benefiting from basic optimizations like instruction scheduling. The 4x (0.212 ms) and 8x (0.177 ms) unrolled versions now perform better, with the 8x version achieving a 2.03x speedup, indicating effective use of instruction-level parallelism. The compiler-optimized version (0.360 ms) remains comparable to the basic loop (1.00x speedup), suggesting the compiler’s optimizations align with the basic loop’s structure.
- **-O3 (Aggressive optimization):** The basic loop drops to 0.178 ms, and the 8x unrolled version becomes the fastest at 0.162 ms (1.10x speedup), likely due to advanced loop unrolling and vectorization by the compiler. The 4x version (0.212 ms) is slower (0.84x), possibly due to suboptimal unrolling depth. The compiler-optimized version (0.172 ms) offers a 1.03x speedup, showing marginal improvement over the basic loop.

### Profiling Results

![O0 Profiling Result](profiling/O0.png)
Elapsed time is 33.307s with 29.274s CPU time. Memory Bound is 1.6% of Pipeline Slots, with L1 Bound at 1.2%, L2 at 0.4%, L3 at 0.2%, DRAM at 0.1%, and Store at 0.2%. DRAM Bandwidth Bound is 0.0% of Elapsed Time, with 123,835,114 loads and 60,243,407 stores. Total Thread Count is 4, with 0s paused time. High memory bound and significant load/store counts suggest cache inefficiency, explaining the poor performance of unrolled loops.


![O2 Profiling Result](profiling/O2.png)
Elapsed time is 1.334s with 1.090s CPU time. Memory Bound is 2.0% of Pipeline Slots, with L1 at 4.5%, L2 at 0.0%, L3 at 0.0%, DRAM at 0.6%, and Store at 0.0%. DRAM Bandwidth Bound is 0.0% of Elapsed Time, with 2,449,273 loads and 35,801,074 stores. Total Thread Count is 1. Reduced memory bound and lower load/store counts indicate better cache utilization, supporting the speedup from unrolling.


![O3 Profiling Result](profiling/O3.png)
Elapsed time is 1.083s with 0.859s CPU time. Memory Bound is 3.6% of Pipeline Slots, with L1 at 2.5%, L2 at 0.0%, L3 at 2.5%, DRAM at 0.6%, and Store at 0.0%. DRAM Bandwidth Bound is 0.0% of Elapsed Time, with 787,023 loads and 12,400,372 stores. Total Thread Count is 2. Lower load/store counts and balanced L1/L3 bounds suggest effective vectorization, aligning with the 8x unrolled version’s best performance.

These results indicate that unrolling benefits increase with optimization levels due to reduced cache misses and better memory access patterns. The compiler-optimized version adapts well across levels, though manual unrolling (8x) excels at -O3 due to aggressive optimization.
