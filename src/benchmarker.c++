#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

#include "se_bench/benchmarker.h++"

using namespace std;

using json = nlohmann::json;

// -----------------------------------------------------------------------------------------------------------------------------
// Mismatch error
// -----------------------------------------------------------------------------------------------------------------------------

struct mismatch_error {
    string key;
    string record;

    string expected_key;

    mismatch_error(string key, string record, string expected_key): key(key), record(record), expected_key(expected_key) {}
};

// -----------------------------------------------------------------------------------------------------------------------------
// Information
// -----------------------------------------------------------------------------------------------------------------------------

string benchmarker::get_working_directory() {
    return this->working_directory;
}

// -----------------------------------------------------------------------------------------------------------------------------
// Opening / Closing
// -----------------------------------------------------------------------------------------------------------------------------

void benchmarker::open() {
    this->ds->open(this->working_directory);
}

void benchmarker::close() {
    this->ds->close();
}

// Preparing (Initializing)

void benchmarker::prepare() {
    this->ds->prepare();
}

// -----------------------------------------------------------------------------------------------------------------------------
// Operation
// -----------------------------------------------------------------------------------------------------------------------------

void benchmarker::batch_seq_write(long from, long to) {
    string key, record;

    // Initializing the batch of keys and records

    int size = to - from + 1;

    string *keys = new string[size];
    string *records = new string[size];

    for (int document_num = from, i = 0; document_num <= to; document_num++, i++) {
        // Initializing Key and Record

        key = to_string(document_num);

        this->document_prototype[this->document_key_name] = key;
        record = this->document_prototype.dump();

        // Adding the Key and the Record to the batch

        keys[i] = key;
        records[i] = record;
    }

    // Writing out the batch

    this->ds->write(size, keys, records);

    // Clearing the keys and records

    delete[] keys;
    delete[] records;
}

void benchmarker::random_read(long limit) {
    // Generating a random key to retrieve within a specific limit
    long key_raw = rand() % limit + 1;

    // Converting into a string (Appropriate type)
    string key = to_string(key_raw);

    // Reading a record which is SUPPOSED to have the appopriate key
    string record = this->ds->read(key);

    // Converting the RAW (String) record into JSON
    json document = json::parse(record);

    // Parsing the record and retrieving the key from it
    string check_key = document[this->document_key_name].get<string>();

    // Checking that the retrieved key si the same as the expected key
    if(check_key != key) {
        cout << "MISMATCH ERROR: " << key << " <-> " << check_key << endl;

        throw mismatch_error(key, record, check_key);
    }
}

// -----------------------------------------------------------------------------------------------------------------------------
// Workloads
// -----------------------------------------------------------------------------------------------------------------------------

// Performing a series of sequential batch-writes
void benchmarker::writes() {
    long from, to;

    for (long document_num = 1; document_num <= this->number_of_documents; document_num += this->write_batch_size) {
        from = document_num;
        to = min(document_num + this->write_batch_size - 1, this->number_of_documents);

        this->batch_seq_write(from, to);
    }
}

// Performing random reads
void benchmarker::reads() {
    for(long document_num = 1; document_num <= this->number_of_documents; document_num++) {
        this->random_read(document_num);
    }
}

// Performing a series of sequential batch-writes followed by random-reads
void benchmarker::mixed() {
    long from, to;

    for (long document_num = 1; document_num <= this->number_of_documents; document_num += this->write_batch_size) {
        // 1. Calculating limits
        from = document_num;
        to = min(document_num + this->write_batch_size - 1, this->number_of_documents);

        // 2. Writing a batch sequentially
        this->batch_seq_write(from, to);

        // 3. Performing random-reads on all records written so far
        for (long i = from; i <= to; i++) {
            random_read(to);
        }
    }
}
