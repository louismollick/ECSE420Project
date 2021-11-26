#!/bin/bash
# Usage: ./runwasm.sh <program_name> <arguments>
# Ex: ./runwasm.sh sequential 1000
# Ex: ./runwasm.sh pthreads 1000 4

emrun --browser chrome ./out/web/$1/index.html $2 $3