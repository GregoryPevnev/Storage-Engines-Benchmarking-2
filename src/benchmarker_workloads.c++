#include <iostream>
#include <string>
#include <chrono>

#include "se_bench/benchmarker.h++"

using namespace std;
using namespace std::chrono;

// -------------------------------------------------------------------------------------------------------------------------------------
// Utilities
// -------------------------------------------------------------------------------------------------------------------------------------

high_resolution_clock::time_point current_time() {
    return high_resolution_clock::now();
}

long time_diff(high_resolution_clock::time_point start_time, high_resolution_clock::time_point end_time) {
    duration<double> time_span = duration_cast<duration<double>>(end_time - start_time);

    double seconds = time_span.count();

    return seconds * 1000;
}

// -------------------------------------------------------------------------------------------------------------------------------------
// Workloads Benchmarking
// -------------------------------------------------------------------------------------------------------------------------------------

// Writing

void benchmark_data_store_for_writes(benchmarker& bench, bool with_info) {
    high_resolution_clock::time_point start_time;
    long total_time;

    // Opening

    bench.open();

    // Workload

    if (with_info) {
        cout << "Writing workload" << endl;
    }

    start_time = current_time();

    bench.writes();

    total_time = time_diff(start_time, current_time());

    // Closing

    bench.close();

    // Information Output

    if (with_info) {
        // Rounding up (At least some time was taken)
        total_time = total_time == 0 ? 1 : total_time;

        cout << "Writing workload total time: " << total_time << " ms" << endl;
    }
}

// Reading

void benchmark_data_store_for_reads(benchmarker& bench, bool with_info) {
    high_resolution_clock::time_point start_time;
    long total_time;

    // Opening

    bench.open();

    // Workload

    if (with_info) {
        cout << "Reading workload" << endl;
    }

    start_time = current_time();

    bench.reads();

    total_time = time_diff(start_time, current_time());

    // Closing

    bench.close();

    // Information Output

    if (with_info) {
        // Rounding up (At least some time was taken)
        total_time = total_time == 0 ? 1 : total_time;

        cout << "Reading workload total time: " << total_time << " ms" << endl;
    }
}

// Mixed

void benchmark_data_store_for_mixed(benchmarker& bench, bool with_info) {
    high_resolution_clock::time_point start_time;
    long total_time;

    // Opening

    bench.open();

    // Workload

    if (with_info) {
        cout << "Mixed operations workload" << endl;
    }

    start_time = current_time();

    bench.mixed();

    total_time = time_diff(start_time, current_time());

    // Closing

    bench.close();

    // Information Output

    if (with_info) {
        // Rounding up (At least some time was taken)
        total_time = total_time == 0 ? 1 : total_time;

        cout << "Mixed workload total time: " << total_time << " ms" << endl;
    }
}