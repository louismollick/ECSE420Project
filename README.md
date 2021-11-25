# ECSE420Project
Are the current solutions for parallelization of programs compiled to WebAssembly helping bridge the performance gap between web and native applications?

### How to compile

1. Install emsdk

2. `emcc c/matrixMultiplication.c -s WASM=1 -o html/index.html`

#### Resources

https://developer.mozilla.org/en-US/docs/WebAssembly/C_to_wasm#compiling_an_example
http://www.sable.mcgill.ca/publications/techreports/2021-4/techrep.pdf