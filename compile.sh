#!/bin/bash
# Usage: ./compile

# Make sure output dir exists
mkdir -p out/native
mkdir -p out/web/sequential
mkdir -p out/web/pthreads

# Clean
rm -f out/*.o

# Compile to native executable
clang -O3 c/sequential.c -o ./out/native/sequential.o
clang -O3 c/pthreads.c -o ./out/native/pthreads.o

# Compile to wasm
emcc --emrun -s ALLOW_MEMORY_GROWTH=1 c/sequential.c -s WASM=1 -o out/web/sequential/index.html
emcc --emrun -pthread -s PROXY_TO_PTHREAD -s PTHREAD_POOL_SIZE=10 -s INITIAL_MEMORY=124MB c/pthreads.c -s WASM=1 -o out/web/pthreads/index.html