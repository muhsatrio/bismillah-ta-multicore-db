#!/bin/bash

g++ -o run.out preprocessing.cpp lib/vertex.cpp lib/vector.cpp lib/output.cpp lib/perpendicular_bisector.cpp lib/point.cpp -l mysqlcppconn
./run.out
