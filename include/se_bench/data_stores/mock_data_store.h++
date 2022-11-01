#ifndef MOCK_DATA_STORE
#define MOCK_DATA_STORE

#include <string>

#include "se_bench/benchmarker.h++"

using namespace std;

class mock_data_store: public data_store {
public:
    mock_data_store(): data_store("mock") {}

    void open(string working_directory_path);

    void prepare();

    string read(string key);

    void write(int count, string* keys, string* records);

    void close();
};

#endif
