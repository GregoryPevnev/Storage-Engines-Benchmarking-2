#ifndef FORESTDB_DATA_STORE
#define FORESTDB_DATA_STORE

#include <string>

#include "libforestdb/forestdb.h"

#include "se_bench/benchmarker.h++"

using namespace std;

class forestdb_data_store: public data_store {
private:
    fdb_file_handle *file_handle;
    fdb_kvs_handle *kvs_handle;
    fdb_status status;
public:
    forestdb_data_store(): data_store("forestdb") {}

    void open(string working_directory_path);

    void prepare();

    string read(string key);

    void write(int count, string* keys, string* records);

    void close();
};

#endif
