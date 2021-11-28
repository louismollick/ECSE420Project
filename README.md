# ECSE420Project
Are the current solutions for parallelization of programs compiled to WebAssembly helping bridge the performance gap between web and native applications?

### How to compile & run

1. Install emsdk: 
    - Either using their instructions (didn't work for me): https://emscripten.org/docs/getting_started/downloads.html
    - Using Homebrew or other equivalent: `brew install emscripten`
    - This is necessary for both compiling & running the WebAssembly code

2. Run `./compile.sh`. This compiles the c code into:
    - a native executable in `out/native`
    - WebAssembly (.wasm) with HTML/Javascript drivers in `out/web`
    - OPTIONALLY: you can use `./compile_docker.sh` which does the same thing, but compiles it inside of a Docker container. This is the best option if you're having issues compiling with your installed emscripten ; but it won't help with `./runwasm.sh` (so you still need to install emscripten locally)

3. Run C executables like normal: `./out/native/sequential.o 1000`

4. Run WebAssembly program using: `./runwasm.sh <program_name> <arguments>`
    - For example `./runwasm.sh sequential 1000`
    - This should open a Chrome window, and the time result should appear in the command line.
    - NOTE: If you'd like to use more than 5 threads, you'll need to set the PTHREAD_POOL_SIZE argument higher in `compile.sh`.

## Problems/Questions observed
- Running any of the programs (sequential.c, pthreads.c, simd_sse.c) in WebAssembly with a high matrix size (~2000) returns the same incorrect value:
```
➜  ECSE420Project git:(main) ✗ ./out/native/pthreads.o 2000 4
2664667000 2662668000 2660669000 2658670000 2656671000 2654672000 2652673000 2650674000 2648675000 2646676000 ...
pthreads.c (2000 x 2000 matrix, 4 threads) - Runtime: 16191705
➜  ECSE420Project git:(main) ✗ ./runwasm.sh pthreads 2000 4
Opening in existing browser session.
-1630300296 -1632299296 -1634298296 -1636297296 -1638296296 -1640295296 -1642294296 -1644293296 -1646292296 -1648291296 ...
pthreads.c (2000 x 2000 matrix, 4 threads) - Runtime: 12081000
```
- This could be insufficient memory allocation on WebAssembly, might need a way to allocate more
- or could be due to some compiler optimization that's breaking the code -- might be possible to fix with some flag

#### Resources

https://emscripten.org/docs/porting/pthreads.html
https://developer.mozilla.org/en-US/docs/WebAssembly/C_to_wasm#compiling_an_example
http://www.sable.mcgill.ca/publications/techreports/2021-4/techrep.pdf
