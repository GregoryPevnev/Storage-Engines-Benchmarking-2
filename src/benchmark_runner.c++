#include <iostream>
#include <cstdlib>
#include <string>
#include <chrono>
#include <thread>

#include "se_bench/benchmarker.h++"
#include "se_bench/file_utilities.h++"

using namespace std;

// Giving profiling some time to start if needed
void wait_before_running(int seconds) {
    this_thread::sleep_for(chrono::milliseconds(seconds * 1000));
}

int run_benchmarking(data_store* ds, int argc, char** argv) {
    // Validating the arguments

    if(argc != 6) {
        cout << "5 arguments are required:" << endl;
        cout << "1) Path to the configuration file" << endl;
        cout << "2) Workload / operation to perform (setup / teardown / writes / reads / mixed)" << endl;
        cout << "3) Number of documents to operate on (Set to 0 if not applicable)" << endl;
        cout << "4) Whether to log or not ('yes')" << endl;
        cout << "5) # of seconds to sleep if needed" << endl;

        return 1;
    }

    // Parsing and checking the arguments

    string config_path = argv[1];
    string workload = argv[2];
    string number_of_documents_str = argv[3];
    string show_info = argv[4];
    string seconds_to_wait_str = argv[5];

    int number_of_documents = stoi(number_of_documents_str);
    int seconds_to_wait = stoi(seconds_to_wait_str);

    bool with_info = show_info == "yes";

    if (number_of_documents <= 0 && workload != "init") {
        cout << "ERROR: Number of documents must be a positive number" << endl;

        throw "Number of documents must be a positive number";
    }

    // Waiting / Sleeping if specified by the input

    if (seconds_to_wait > 0) {
        cout << "Waiting for " << seconds_to_wait << " seconds..." << endl;

        wait_before_running(seconds_to_wait);

        cout << "Waiting completed" << endl;
    }

    // Building configuration from the config-file and arguments

    benchmark_config config = load_benchmarking_config(config_path);

    config.number_of_documents = number_of_documents;

    config.working_directory = get_working_directory(config.data_directory, ds->get_name());

    // Initializing the Benchmarker

    benchmarker bench(config.number_of_documents, config.write_batch_size, config.document_prototype, config.document_key_name, config.working_directory, ds);

    // Seeding random numbers

    srand((unsigned) time(NULL));

    // Running the specified operation

    if (workload == "init") {
        initialize_data_store(bench, with_info);
    } else if (workload == "writes") {
        benchmark_data_store_for_writes(bench, with_info);
    } else if (workload == "reads") {
        benchmark_data_store_for_reads(bench, with_info);
    } else if (workload == "mixed") {
        benchmark_data_store_for_mixed(bench, with_info);
    } else {
        cout << "ERROR: Invalid workload / operation" << endl;

        throw "Invalid workload / operation";
    }

    if (with_info) {
        cout << "Finished" << endl;
    }

    return 0;
}
