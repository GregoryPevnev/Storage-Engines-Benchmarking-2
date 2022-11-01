#ifndef FILE_UTILS
#define FILE_UTILS

#include <string>

using namespace std;

string get_working_directory(string base_dir, string se_dir);

void create_working_directory(string working_directory);

void delete_working_directory(string working_directory);

string get_full_path(string working_directory, string working_path);

#endif
