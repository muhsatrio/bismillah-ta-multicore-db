#!/bin/bash

mpic++ test.cpp lib/data.cpp lib/output.cpp lib/perpendicular_bisector.cpp lib/point.cpp -l mysqlcppconn
mpiexec -n 5 ./a.out
