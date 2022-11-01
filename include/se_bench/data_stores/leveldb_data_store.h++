#ifndef LEVELDB_DATA_STORE
#define LEVELDB_DATA_STORE

#include <string>

#include "leveldb/db.h"

#include "se_bench/benchmarker.h++"

using namespace std;

class leveldb_data_store: public data_store {
private:
    leveldb::DB* db;
    leveldb::Status status;
public:
    leveldb_data_store(): data_store("leveldb") {}

    void open(string working_directory_path);

    void prepare();

    string read(string key);

    void write(int count, string* keys, string* records);

    void close();
};

#endif
