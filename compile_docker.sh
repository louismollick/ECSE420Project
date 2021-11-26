#!/bin/bash
# Usage: ./compile

# Make sure output dir exists
mkdir -p out/native
mkdir -p out/web/sequential
mkdir -p out/web/pthreads

# Clean
rm -f out/*.o

# Compile to native executable
gcc c/sequential.c -o ./out/native/sequential.o
gcc c/pthreads.c -o ./out/native/pthreads.o

# Compile to wasm
docker run \
  --rm \
  -v $(pwd):/src \
  -u $(id -u):$(id -g) \
  emscripten/emsdk \
  emcc --emrun -s ALLOW_MEMORY_GROWTH=1 c/sequential.c -s WASM=1 -o out/web/sequential/index.html

docker run \
  --rm \
  -v $(pwd):/src \
  -u $(id -u):$(id -g) \
  emscripten/emsdk \
  emcc --emrun -pthread -s PROXY_TO_PTHREAD -s PTHREAD_POOL_SIZE=10 -s ALLOW_MEMORY_GROWTH=1 c/pthreads.c -s WASM=1 -o out/web/pthreads/index.html
