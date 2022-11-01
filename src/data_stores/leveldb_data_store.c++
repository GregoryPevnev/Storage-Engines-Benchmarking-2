#include <string>

#include "leveldb/db.h"
#include "leveldb/write_batch.h"

#include "se_bench/data_stores/leveldb_data_store.h++"
#include "se_bench/file_utilities.h++"

using namespace std;

// Properties:
//   1) Using the Clustered-Index File-Organization for storage: LSM Tree
//   2) Performing insertions in batches
//   3) Using Asynchronous Disk-IO to optimize write-performance
//   4) No journalling at all due to the specifics of the Data-Structure -> No overheads in terms of writes and disk IO

void leveldb_data_store::open(string working_directory) {
    string db_path = get_full_path(working_directory, "bench");

    leveldb::Options options;
    options.create_if_missing = true;

    this->status = leveldb::DB::Open(options, db_path, &this->db);

    if(!status.ok()) {
        string err = status.ToString();

        throw err;
    }
}

// No preparation required
void leveldb_data_store::prepare() {}

string leveldb_data_store::read(string key) {
    leveldb::ReadOptions options;

    string value;

    this->status = this->db->Get(options, key, &value);

    if(!status.ok()) {
        string err = status.ToString();

        throw err;
    }

    return value;
}

void leveldb_data_store::write(int count, string* keys, string* records) {
    // Performance: Using asynchronous writes
    leveldb::WriteOptions options;
    options.sync = false;

    // Batch: Building a batch of key-value pairs to write
    leveldb::WriteBatch batch;
    for (int i = 0; i < count; i++) batch.Put(keys[i], records[i]);

    this->status = this->db->Write(options, &batch);

    if(!status.ok()) {
        string err = status.ToString();

        throw err;
    }
}

void leveldb_data_store::close() {
    delete this->db;
}
