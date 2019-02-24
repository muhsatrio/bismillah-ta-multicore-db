#!/bin/bash

mpic++ -o run.out main.cpp -l mysqlcppconn
mpiexec -n 10 ./run.out

 