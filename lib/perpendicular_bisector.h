#ifndef PERPENDICULAR_BISECTOR_H
#define PERPENDICULAR_BISECTOR_H

#include <vector>
#include "data.h"
#include <iostream>
#include "mpich/mpi.h"

using namespace std;

typedef vector<point> vp;
typedef vector<vp> vvp;

point pb_calculate_mid(point point_1, point point_2);

double pb_calculate_c(double slope, point mid_point);

line pb_compute(point point_1, point point_2);

vector<line> pb_generate(point query_point, vector<point> interest_point);

double pb_calculate_slope(point point_1, point point_2);

#endif