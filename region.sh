#!/bin/bash
g++ create_table_region.cpp -l mysqlcppconn
./a.out
mpic++ -o region.out construct_region.cpp lib/segment.cpp -l mysqlcppconn
mpiexec -n 8 ./region.out
