#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <numeric>
#include <functional>

const int ARRAY_SIZE = 1000000;
const int ITERATIONS = 1000;

int sum_squares_basic(const std::vector<int>& data) {
    int sum = 0;
    for (size_t i = 0; i < data.size(); ++i) {
        sum += data[i] * data[i];
    }
    return sum;
}

// Manual 4x unrolling
int sum_squares_unrolled_4x(const std::vector<int>& data) {
    int sum = 0;
    size_t size = data.size();
    size_t i = 0;
    
    for (i = 0; i < size - 3; i += 4) {
        sum += data[i] * data[i];
        sum += data[i + 1] * data[i + 1];
        sum += data[i + 2] * data[i + 2];
        sum += data[i + 3] * data[i + 3];
    }
    
    for (; i < size; ++i) {
        sum += data[i] * data[i];
    }
    
    return sum;
}

// Manual 8x unrolling
int sum_squares_unrolled_8x(const std::vector<int>& data) {
    int sum = 0;
    size_t size = data.size();
    size_t i = 0;
    
    for (i = 0; i < size - 7; i += 8) {
        sum += data[i] * data[i];
        sum += data[i + 1] * data[i + 1];
        sum += data[i + 2] * data[i + 2];
        sum += data[i + 3] * data[i + 3];
        sum += data[i + 4] * data[i + 4];
        sum += data[i + 5] * data[i + 5];
        sum += data[i + 6] * data[i + 6];
        sum += data[i + 7] * data[i + 7];
    }
    
    for (; i < size; ++i) {
        sum += data[i] * data[i];
    }
    
    return sum;
}

// Compiler-optimized version using std::accumulate
int sum_squares_compiler(const std::vector<int>& data) {
    return std::accumulate(data.begin(), data.end(), 0, 
        [](int acc, int x) { return acc + x * x; });
}

auto get_time_ms(const std::vector<int>& data, std::function<int(const std::vector<int>&)> func) {
    int sum = 0;
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) {
        sum = func(data);
    }
    auto end = std::chrono::steady_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / static_cast<double>(ITERATIONS);
    return std::make_pair(sum, time);
}

int main() {
    std::cout << "=== Sum of Squares Loop Unrolling Test ===\n";
    std::cout << "Array size: " << ARRAY_SIZE << " elements\n";
    std::cout << "Iterations: " << ITERATIONS << "\n\n";
    
    std::vector<int> data(ARRAY_SIZE);
    for (size_t i = 0; i < ARRAY_SIZE; ++i) {
        data[i] = (i % 100) / 10 + 1;
    }
    
    // Test all versions
    auto [sum_basic, time_basic] = get_time_ms(data, sum_squares_basic);
    auto [sum_4x, time_4x] = get_time_ms(data, sum_squares_unrolled_4x);
    auto [sum_8x, time_8x] = get_time_ms(data, sum_squares_unrolled_8x);
    auto [sum_compiler, time_compiler] = get_time_ms(data, sum_squares_compiler);
    
    std::cout << "\nResults (average time per iteration):\n";
    std::cout << "Basic loop:           " << std::fixed << std::setprecision(3) << time_basic 
              << " ms (result: " << sum_basic << ")\n";
    std::cout << "4x unrolled:          " << std::setprecision(3) << time_4x 
              << " ms (result: " << sum_4x << ")\n";
    std::cout << "8x unrolled:          " << std::setprecision(3) << time_8x 
              << " ms (result: " << sum_8x << ")\n";
    std::cout << "Compiler optimized:   " << std::setprecision(3) << time_compiler 
              << " ms (result: " << sum_compiler << ")\n";

    std::cout << "\nSpeedup vs basic loop:\n";
    std::cout << "4x unrolled:          " << std::setprecision(2) << time_basic / time_4x << "x\n";
    std::cout << "8x unrolled:          " << std::setprecision(2) << time_basic / time_8x << "x\n";
    std::cout << "Compiler optimized:   " << std::setprecision(2) << time_basic / time_compiler << "x\n";
    return 0;
}