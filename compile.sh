#!/bin/bash
# Usage: ./compile

# Make sure output dir exists
mkdir -p out/native
mkdir -p out/web/sequential
mkdir -p out/web/pthreads
mkdir -p out/web/simd_sse

# Clean
rm -f out/*.o

# Compile to native executable
clang -O3 c/sequential.c -o ./out/native/sequential.o
clang -O3 c/pthreads.c -o ./out/native/pthreads.o
clang -O3 c/simd_sse.c -o ./out/native/simd_sse.o

# Compile to wasm
emcc --emrun -s ALLOW_MEMORY_GROWTH=1 c/sequential.c -s WASM=1 -o out/web/sequential/index.html
emcc --emrun -pthread -s PROXY_TO_PTHREAD -s PTHREAD_POOL_SIZE=10 -s ALLOW_MEMORY_GROWTH=1 c/pthreads.c -s WASM=1 -o out/web/pthreads/index.html
emcc --emrun -msimd128 -mavx -s ALLOW_MEMORY_GROWTH=1 c/simd_sse.c -s WASM=1 -o out/web/simd_sse/index.html