#!/bin/bash

# Specifying storage engines to be used for benchmarking
STORAGE_ENGINES=( "leveldb" "forestdb" "sqlite3" )

# Sizes of test workloads to be used for running the benchmarking on
SIZES=( 10000 25000 50000 100000 200000 )

# Workloads to test
WORKLOADS=( writes reads mixed )

# Path for outputting the benchmark metrics
BENCHMARKING_METRICS_PATH="./metrics"

# Configuration path
CONFIG_PATH=config.json


# Preparing a corresponding directory for the output (Clearing first)
rm -rf $BENCHMARKING_METRICS_PATH
mkdir -p $BENCHMARKING_METRICS_PATH

# Running the benchmarking and outputting to a file (With additional cleanup of previous benchmarks)

echo "Benchmarking speed and throughput"

rm -rf "$BENCHMARKING_METRICS_PATH"

for se in "${STORAGE_ENGINES[@]}"
do
  echo -e "\nStorage Engine:"
  echo "$se" | awk '{print toupper($0)}'

  for s in "${SIZES[@]}"
  do
    for w in "${WORKLOADS[@]}"
    do
      target="./build/bin/bench_$se"

      # Resetting / Initializing the data-store for ALL workloads that are NOT reads
      #    - Depend on the previously written data
      if [ "$w" != "reads" ]
        then
          $target $CONFIG_PATH init 0 no 0
      fi

      echo "Workload \"$w\" of size $s"

      mkdir -p "$BENCHMARKING_METRICS_PATH/$se/$w/$s"

      $target $CONFIG_PATH $w $s yes 0 > "$BENCHMARKING_METRICS_PATH/$se/$w/$s/benchmarks"
    done
  done

  echo -e "---------------------------------------------------------"
done

echo "Finished benchmarking all storage engines"
