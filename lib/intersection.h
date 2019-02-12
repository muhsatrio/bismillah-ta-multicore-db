#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "data.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include "mpich/mpi.h"

using namespace std;

bool int_rules_sorting(point a, point b);

vector<point> int_sorting(vector<point> points);

vector<vector<point>> int_generate(vector<line> lines, double bound);

// void i_generate(vector<vector<point>> intersects, vector<line> lines, double bound);

vector<point> int_generate_partial(vector<line> lines, double bound, unsigned int index);

double int_calculate_x(line line_1, line line_2);

point int_compute(line line_1, line line_2);

vector<point> int_compute_bound(line line_, double bound);

vector<point> int_remove_outside_bound(vector<point> points, double bound); 

bool int_line_segment(line line_, segment segment_);
unsigned int int_calculate_orientation(point point_1, point point_2, point point_3);


bool int_segment(segment segment_1, segment segment_2);

#endif