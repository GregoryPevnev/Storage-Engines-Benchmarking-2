#ifndef SQLITE3_DATA_STORE
#define SQLITE3_DATA_STORE

#include <string>

#include "sqlite3.h"

#include "se_bench/benchmarker.h++"

using namespace std;

class sqlite3_data_store: public data_store {
private:
    sqlite3 *db;
public:
    sqlite3_data_store(): data_store("sqlite3") {}

    void open(string working_directory_path);

    void prepare();

    string read(string key);

    void write(int count, string* keys, string* records);

    void close();
};

#endif
