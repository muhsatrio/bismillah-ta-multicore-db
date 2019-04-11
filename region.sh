#!/bin/bash
g++ create_table_region.cpp -l mysqlcppconn
./a.out
mpic++ construct_region.cpp lib/segment.cpp -l mysqlcppconn
mpiexec -n 10 ./a.out