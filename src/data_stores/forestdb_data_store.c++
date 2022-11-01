#include <string>

#include "libforestdb/forestdb.h"

#include "se_bench/data_stores/forestdb_data_store.h++"
#include "se_bench/file_utilities.h++"

using namespace std;

// Properties:
//   1) Using the Clustered-Index File-Organization for storage: HB+- Trie
//   2) Performing insertions in batches
//   3) Using Asynchronous Disk-IO to optimize write-performance
//   4) Configuring to only flush WAL before committing -> Minimizing Disk IO

void forestdb_data_store::open(string working_directory) {
    string db_path = get_full_path(working_directory, "bench");
    
    fdb_config config = fdb_get_default_config();
    config.durability_opt = FDB_DRB_ASYNC;
    config.wal_flush_before_commit = true;

    fdb_kvs_config kvs_config = fdb_get_default_kvs_config();

    this->status = fdb_open(&this->file_handle, db_path.c_str(), &config);

    if (this->status != FDB_RESULT_SUCCESS) {
        throw "Could not open a ForestDB database file";
    }

    this->status = fdb_kvs_open_default(this->file_handle, &this->kvs_handle, &kvs_config);

    if (this->status != FDB_RESULT_SUCCESS) {
        throw "Could not open a ForestDB key-value store";
    }
}

// No preparation required
void forestdb_data_store::prepare() {}

string forestdb_data_store::read(string key) {
    size_t value_len;
    void *value;

    this->status = fdb_get_kv(this->kvs_handle, key.c_str(), key.size(), &value, &value_len);

    if (this->status != FDB_RESULT_SUCCESS) {
        throw "Could not read a key-value pair in ForestDB";
    }

    string str_value((char*) value, value_len);

    return str_value;
}

void forestdb_data_store::write(int count, string* keys, string* records) {
    string key, record;

    for (int i = 0; i < count; i++) {
        key = keys[i];
        record = records[i];

        this->status = fdb_set_kv(this->kvs_handle, key.c_str(), key.size(), record.c_str(), record.size());

        if (this->status != FDB_RESULT_SUCCESS) {
            throw "Could not write a key-value pair in ForestDB";
        }
    }

    this->status = fdb_commit(this->file_handle, FDB_COMMIT_NORMAL);

    if (this->status != FDB_RESULT_SUCCESS) {
        throw "Could not commit writing of a key-value pair in ForestDB";
    }
}

void forestdb_data_store::close() {
    this->status = fdb_close(this->file_handle);

    if (this->status != FDB_RESULT_SUCCESS) {
        throw "Could not close a ForestDB database file";
    }
}
