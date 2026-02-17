#!/bin/bash
if [ ! -d "./limine" ]; then
    ./setup.sh
fi

rm bin/image.hdd
rm bin/LyraeOS
make run
