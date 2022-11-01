#include <iostream>
#include <string>

#include "se_bench/benchmarker.h++"
#include "se_bench/file_utilities.h++"

using namespace std;

// Initializing / Resetting the data-store

void initialize_data_store(benchmarker& bench, bool with_info) {
    // IMPORTANT: TEARING DOWN PREVIOUS DATA JUST IN CASE

    if (with_info) {
        cout << "Tearing down the data-store files" << endl;
    }

    delete_working_directory(bench.get_working_directory());

    // Creating a data-directory for storing the data

    if (with_info) {
        cout << "Setting up the data-store files" << endl;
    }

    create_working_directory(bench.get_working_directory());

    // Opening

    bench.open();

    // Preparation

    if (with_info) {
        cout << "Preparing" << endl;
    }

    bench.prepare();

    // Closing

    bench.close();
}
