#!/bin/bash
# LINUX BULD
cmake -S . -B build
cmake --build build
./build/sced
