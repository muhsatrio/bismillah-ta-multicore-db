#!/bin/bash

mpic++ -o run.out main.cpp lib/data.cpp lib/vertex.cpp lib/vector.cpp lib/output.cpp lib/perpendicular_bisector.cpp lib/point.cpp -l mysqlcppconn
mpiexec -n 10 ./run.out
