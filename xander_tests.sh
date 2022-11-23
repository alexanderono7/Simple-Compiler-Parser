#!/bin/bash

for filename in provided_tests/*.txt; do
    echo "executing" + $filename
    ./a.out < $filename
    echo -e "\n"
done