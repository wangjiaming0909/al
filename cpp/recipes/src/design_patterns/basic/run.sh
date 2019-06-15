#! /bin/bash

while true
do
    ./recipes
    if [ $? -ne 0 ]; then
        break;
    fi
done
