#!/bin/bash

g++ test.cpp lib/data.cpp lib/output.cpp lib/vertex.cpp lib/perpendicular_bisector.cpp lib/point.cpp lib/vector.cpp lib/region.cpp -l mysqlcppconn
./a.out
