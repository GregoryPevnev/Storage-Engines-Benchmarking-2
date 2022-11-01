#!/bin/bash

# Configuration path
CONFIG_PATH=config.json

STORAGE_ENGINE="$1"

case $STORAGE_ENGINE in

  mock)
    echo "Mock Storage Engine"
    TARGET="./build/bin/bench_mock"
    ;;

  sqlite3)
    echo "SQLite3"
    TARGET="./build/bin/bench_sqlite3"
    ;;

  forestdb)
    echo "ForestDB"
    TARGET="./build/bin/bench_forestdb"
    ;;

  leveldb)
    echo "LevelDB"
    TARGET="./build/bin/bench_leveldb"
    ;;

  *)
    echo "INVALID STORAGE ENGINE PROVIDED"
    exit 1
    ;;
esac

WORKLOAD="$2"

if [ -z "$WORKLOAD" ]
  then
    echo "WORKLOAD NOT SUPPLIED"
    exit 1
fi

if [ "$WORKLOAD" != "writes" ] && [ "$WORKLOAD" != "reads" ] && [ "$WORKLOAD" != "mixed" ];
  then
    echo "INVALID WORKLOAD"
    exit 1
fi

SIZE="$3"

if [ -z "$SIZE" ]
  then
    echo "INVALID SIZE SUPPLIED"
    exit 1
fi

echo "Benchmarking speed of a workload of $SIZE document(s) performing $WORKLOAD"

# Setting up (If performing a non-read workload)
if [ "$WORKLOAD" != "reads" ]
  then
    ./$TARGET $CONFIG_PATH init 0 yes 0
fi

# Tearing down
./$TARGET $CONFIG_PATH $WORKLOAD $SIZE yes 0