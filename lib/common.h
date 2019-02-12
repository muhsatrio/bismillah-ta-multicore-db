#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include "data.h"
#include <iostream>
#include "mpich/mpi.h"

using namespace std;

unsigned int com_average_segments(vector<vector<segment>> influence_zone);

#endif