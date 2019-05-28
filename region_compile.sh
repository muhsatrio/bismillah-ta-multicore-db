#!/bin/bash

g++ --std=c++11 -o create.out create_table_region.cpp -l mysqlcppconn
mpic++ --std=c++11 construct_region.cpp lib/segment.cpp -l mysqlcppconn
g++ --std=c++11 -o recovery.out recovery_segment.cpp lib/segment.cpp -l mysqlcppconn