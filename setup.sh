#!/bin/bash
if [ -d "./limine" ]; then
	echo "not downloading limine, already exists"
else
	echo "downloading limine"
	git clone https://codeberg.org/Limine/Limine.git limine --branch=v10.x-binary --depth=1
    make -C limine
fi
