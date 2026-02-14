#!/bin/bash
if [ ! -d "./limine" ]; then
    ./setup.sh
fi

make clean
make run
