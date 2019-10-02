#!/bin/bash

docker build . --tag $1/so_builder

docker run -v ${PWD}:/sources/ $1/so_builder

if [[ "$2" == "gdb" ]]; then
	./run.sh gdb
else
	./run.sh
fi