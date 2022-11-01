#include <string>
#include <ctime>
#include <filesystem>

namespace fs = std::__fs::filesystem;

using namespace std;

string get_working_directory(string base_dir, string se_dir) {
    fs::path cur_dir = fs::current_path();

    fs::path working_directory_path = cur_dir / base_dir / se_dir;

    return working_directory_path.u8string();
}

void create_working_directory(string working_directory) {
    fs::path working_directory_path = fs::u8path(working_directory);

    fs::create_directories(working_directory_path);
}

void delete_working_directory(string working_directory) {
    fs::path working_directory_path = fs::u8path(working_directory);

    fs::remove_all(working_directory_path);
}

string get_full_path(string working_directory, string working_path) {
    fs::path wd_path(working_directory);
    fs::path wp_path(working_path);
    fs::path full_path = wd_path / wp_path;

    return full_path.u8string();
}
