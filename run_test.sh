#!/bin/bash

g++ test.cpp lib/output.cpp lib/vertex.cpp lib/perpendicular_bisector.cpp lib/point.cpp lib/segment.cpp -l mysqlcppconn
./a.out
