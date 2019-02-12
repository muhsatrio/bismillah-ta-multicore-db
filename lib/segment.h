#ifndef SEGMENT_H
#define SEGMENT_H

#include "data.h"
#include <vector>
#include <iostream>

using namespace std;

vector<segment> seg_compute(vector<point> points, unsigned int from);

vector<segment> seg_generate(vector<vector<point>> points);

point seg_mid_point(segment sgmnt);

#endif