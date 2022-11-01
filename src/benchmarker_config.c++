#include <fstream>
#include <streambuf>
#include <string>
#include <nlohmann/json.hpp>

#include "se_bench/benchmarker.h++"

using namespace std;

using json = nlohmann::json;

string load_config_from_file(string config_path) {
    ifstream t(config_path);

    string str((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());

    return str;
}

benchmark_config load_benchmarking_config(string config_path) {
    string config_data_str = load_config_from_file(config_path);

    json config_data = json::parse(config_data_str);

    benchmark_config config;

    config.number_of_documents = -1; // Has to be supplied from the command-line

    config.write_batch_size = config_data["write_batch_size"].get<int>();

    config.data_directory = config_data["data_directory_name"].get<string>();
    config.working_directory = ""; // Set based on the chosen data-store

    config.document_key_name = config_data["document_key_name"].get<string>();
    config.document_prototype = config_data["document_prototype"];

    return config;
}
