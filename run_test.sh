#!/bin/bash
mpic++ test.cpp
mpiexec -n 5 ./a.out
