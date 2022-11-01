cmake_minimum_required(VERSION 3.10)

project(Tester)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Requirements:
#    1) Using a CMAKE utility that supports vertsion 3.10
#    2) Using a C++ compiler that supports version 14
#    3) Having nlohmann/json installed (https://github.com/nlohmann/json) -> Headers added to Include-Path and Library available globally
#    4) Having SQLite3 installed (https://github.com/sqlite/sqlite) -> Headers added to Include-Path and Library available globally
#    5) Having ForestDB installed (https://github.com/couchbase/forestdb) -> Headers added to Include-Path and Library available at a certain location (SET THE VARIABLE)
#    6) Having LevelDB installed (https://github.com/google/leveldb) -> Headers added to Include-Path and Library available at a certain location (SET THE VARIABLE)
#    7) Installing IOSnoop Utility

find_package(nlohmann_json 3.2.0 REQUIRED)
find_package (SQLite3 3.0.0 REQUIRED)

set (ForestDB /usr/local/lib/libforestdb.dylib) # Configuration: Specify the path pointing to the location of the ForestDB library
set (LevelDB /usr/local/lib/libleveldb.dylib) # Configuration: Specify the path pointing to the location of the LevelDB library

set (IncludePath /usr/local/inlcude) # Configuration: Specify the path pointing to the global Include-Path with the Headers

# Mock Storage-Engine for testing the operations of the program
# add_executable(
#     bench_mock
#     src/runners/mock_benchmark_runner.c++
#     src/data_stores/mock_data_store.c++
#     src/benchmarker.c++
#     src/benchmarker_config.c++
#     src/benchmarker_utils.c++
#     src/benchmarker_workloads.c++
#     src/benchmark_runner.c++
#     src/file_utilities.c++
# )
# target_include_directories(bench_mock PUBLIC ${IncludePath} ./include)
# target_link_libraries(bench_mock PRIVATE nlohmann_json::nlohmann_json)

# SQLite3 Storge-Engine benchmarking setup
add_executable(
    bench_sqlite3
    src/runners/sqlite3_benchmark_runner.c++
    src/data_stores/sqlite3_data_store.c++
    src/benchmarker.c++
    src/benchmarker_config.c++
    src/benchmarker_utils.c++
    src/benchmarker_workloads.c++
    src/benchmark_runner.c++
    src/file_utilities.c++
)
target_include_directories(bench_sqlite3 PUBLIC ${IncludePath} ./include)
target_link_libraries(bench_sqlite3 PRIVATE sqlite3 nlohmann_json::nlohmann_json)

# ForestDB Storge-Engine benchmarking setup
add_executable(
    bench_forestdb
    src/runners/forestdb_benchmark_runner.c++
    src/data_stores/forestdb_data_store.c++
    src/benchmarker.c++
    src/benchmarker_config.c++
    src/benchmarker_utils.c++
    src/benchmarker_workloads.c++
    src/benchmark_runner.c++
    src/file_utilities.c++
)
target_include_directories(bench_forestdb PUBLIC ${IncludePath} ./include)
target_link_libraries(bench_forestdb PRIVATE ${ForestDB} nlohmann_json::nlohmann_json)

# LevelDB Storge-Engine benchmarking setup
add_executable(
    bench_leveldb
    src/runners/leveldb_benchmark_runner.c++
    src/data_stores/leveldb_data_store.c++
    src/benchmarker.c++
    src/benchmarker_config.c++
    src/benchmarker_utils.c++
    src/benchmarker_workloads.c++
    src/benchmark_runner.c++
    src/file_utilities.c++
)
target_include_directories(bench_leveldb PUBLIC ${IncludePath} ./include)
target_link_libraries(bench_leveldb PRIVATE ${LevelDB} nlohmann_json::nlohmann_json)
