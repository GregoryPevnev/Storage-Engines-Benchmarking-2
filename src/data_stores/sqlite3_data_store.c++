#include <iostream>
#include <string>

#include "sqlite3.h"

#include "se_bench/data_stores/sqlite3_data_store.h++"
#include "se_bench/file_utilities.h++"

using namespace std;

// Properties:
//   1) Using the Clustered-Index File-Organization for storage: B+- Tree
//   2) Performing insertions in batches (NOT GUARANTEED TO WORK)
//   3) Using Sycnhronous Disk-IO (NO CHOICE)
//   4) Flushing Journal at arbitrary points in time (NO CONTROLs)

// SQL

string setup_sql() {
    // Creating the table as a B-Tree that has the "key" field as the key for organization -> Clustered-Index
    return "CREATE TABLE store(key TEXT NOT NULL PRIMARY KEY, record TEXT NOT NULL) WITHOUT ROWID;";
}

string selection_sql(string key) {
    return "SELECT record FROM store WHERE key = '" + key + "';";
}

string insertion_sql(int count, string* keys, string* records) {
    string sql = "BEGIN;INSERT INTO store(key, record) VALUES ";

    for (int i = 0; i < count; i++) {
        sql += "('" + keys[i] + "', '" + records[i] + "')";

        if (i != count - 1) {
            sql += ", ";
        }
    }

    sql += ";COMMIT;";

    return sql;
}

// Query Processing

// Called for EACH resulting record
static int selection_callback(void* data, int columns, char** values, char** column_names) {
    string* result_data = (string*) data;

    // Only taking the first value - Others are UNEEDED
    *result_data = values[0];

    return 0;
}

// Data Store

void sqlite3_data_store::open(string working_directory) {
    string db_path = get_full_path(working_directory, "bench.db");

    int rc = sqlite3_open(db_path.c_str(), &this->db);

    if(rc) {
        throw "Could not open SQLite database";
    }
}

void sqlite3_data_store::prepare() {
    char* error_message;

    int rc = sqlite3_exec(this->db, setup_sql().c_str(), NULL, 0, &error_message);

    if (rc) {
        cout << "SQLITE TABLE-CREATION ERROR: " << error_message << endl;

        throw error_message;
    }
}

string sqlite3_data_store::read(string key) {
    char* error_message;
    string value = "";

    int rc = sqlite3_exec(this->db, selection_sql(key).c_str(), selection_callback, &value, &error_message);

    if (rc) {
        cout << "SQLITE READ ERROR: " << error_message << endl;

        throw error_message;
    }

    if (value == "") {
        throw "NOT FOUND";
    }

    return value;
}

void sqlite3_data_store::write(int count, string* keys, string* records) {
    char* error_message;

    int rc = sqlite3_exec(this->db, insertion_sql(count, keys, records).c_str(), NULL, 0, &error_message);

    if (rc) {
        cout << "SQLITE WRITE ERROR: " << error_message << endl;

        throw error_message;
    }
}

void sqlite3_data_store::close() {
    sqlite3_close(this->db);
}
