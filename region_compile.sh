#!/bin/bash

g++ -o create.out create_table_region.cpp -l mysqlcppconn
mpic++ construct_region.cpp lib/segment.cpp -l mysqlcppconn
g++ -o recovery.out recovery_segment.cpp lib/segment.cpp -l mysqlcppconn