#!/bin/bash

g++ --std=c++11 -o run.out preprocessing.cpp lib/vertex.cpp lib/segment.cpp lib/output.cpp lib/perpendicular_bisector.cpp lib/point.cpp -l mysqlcppconn
./run.out
