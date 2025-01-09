#!/bin/bash

g++ prev.cpp test.cpp -o test

for((i=0; ;i++)) do
    ./test $i
    result=$?

    if [[ $result -eq 0 ]]; then
        echo "nie dziala"
        exit 0
    elif [[ $result -eq 1 ]]; then
        echo $i "gitara"
    fi
done
