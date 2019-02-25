#!/bin/bash

mpic++ -o run.out main.cpp lib/data.cpp lib/output.cpp lib/point.cpp -l mysqlcppconn
mpiexec -n 10 ./run.out

 