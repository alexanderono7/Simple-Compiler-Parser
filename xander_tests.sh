#!/bin/bash

# For each testcase input in provided_tests do...
for filename in provided_tests/*.txt; do
    echo "executing" + $filename
    ./a.out < $filename
    echo -e "\n"
done