#ifndef BENCHMARKER
#define BENCHMARKER

#include <string>
#include <filesystem>
#include <nlohmann/json.hpp>

namespace fs = std::__fs::filesystem;

using json = nlohmann::json;

using namespace std;

class data_store {
protected:
    string name;
public:
    data_store(string name): name(name) {}

    inline string get_name() {
        return this->name;
    }

    virtual void open(string working_directory_path) = 0;

    virtual void prepare() = 0;

    virtual string read(string key) = 0;

    virtual void write(int count, string* keys, string* records) = 0;

    virtual void close() = 0;
};

struct benchmark_config {
    long number_of_documents;

    int write_batch_size;

    string data_directory; // Directory with ALL data-stores data
    string working_directory; // Current Working directory

    string document_key_name;
    json document_prototype;
};

class benchmarker {
private:
    long number_of_documents;

    int write_batch_size;

    json document_prototype;
    string document_key_name;

    string working_directory;

    data_store* ds;

    void batch_seq_write(long from, long to); // Helper for performing a sequential batch write
    void random_read(long limit); // Helper for performing a random read
public:
    benchmarker(long number_of_documents, int write_batch_size, json document_prototype, string document_key_name, string working_directory, data_store* ds):
        number_of_documents(number_of_documents),
        write_batch_size(write_batch_size),
        document_prototype(document_prototype),
        document_key_name(document_key_name),
        working_directory(working_directory),
        ds(ds) {}

    string get_working_directory();

    void open(); // Preparing for reading and writing (Connections and clients)
    void close(); // Cleaning up the connections and clients

    void prepare(); // Preparing the data-store (Database creation / Key-Value store creation / etc.)

    void writes(); // Writing the specified number of documents according to the prototype => Write performance
    void reads(); // Reading all the written documents and verifying their correctness => Read performance
    void mixed(); // Performing mixed operations (Writing -> Reading) => Mixed performance
};

// Common

benchmark_config load_benchmarking_config(string config_path);

// Utilities

void initialize_data_store(benchmarker& bench, bool with_info);

// Workloads Benchmarking

void benchmark_data_store_for_writes(benchmarker& bench, bool with_info);

void benchmark_data_store_for_reads(benchmarker& bench, bool with_info);

void benchmark_data_store_for_mixed(benchmarker& bench, bool with_info);

#endif
