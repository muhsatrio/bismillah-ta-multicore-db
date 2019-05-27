#!/bin/bash
./create.out
mpiexec -n 5 ./a.out
./recovery.out
mpiexec -n 6 ./a.out
./recovery.out
mpiexec -n 7 ./a.out
./recovery.out
mpiexec -n 8 ./a.out
./recovery.out
mpiexec -n 9 ./a.out
./recovery.out
mpiexec -n 10 ./a.out
./recovery.out
