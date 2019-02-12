#!/bin/bash

mpic++ -o run.out main.cpp lib/common.cpp lib/data.cpp lib/db.cpp lib/influence_zone.cpp lib/intersection.cpp lib/output.cpp lib/perpendicular_bisector.cpp lib/randomize.cpp lib/segment.cpp -l mysqlcppcon
mpiexec -n 6 ./run.out

 