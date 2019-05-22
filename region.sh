#!/bin/bash
g++ -o create.out create_table_region.cpp -l mysqlcppconn
mpic++ -o region.out construct_region.cpp lib/segment.cpp -l mysqlcppconn
g++ -o recovery.out recovery_segment.cpp lib/segment.cpp
./create.out
mpiexec -n 2 ./region.out < input1.in
./recovery.out < input1.in
mpiexec -n 3 ./region.out < input1.in
./recovery.out < input1.in
mpiexec -n 4 ./region.out < input1.in


