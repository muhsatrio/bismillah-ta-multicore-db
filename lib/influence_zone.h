#ifndef INFLUENCE_ZONE_H
#define INFLUENCE_ZONE_H

#include <vector>
#include <tuple>
#include "data.h"
#include "intersection.h"
#include "perpendicular_bisector.h"
#include "segment.h"
#include "output.h"
#include "mpich/mpi.h"
#include <iostream>

using namespace std;

vector<vector<segment>> inf_compute(int counts, point query_point, vector<point> interest_points, double bound);

void inf_compute_partial(point query_point, vector<point> interest_points, double bound);

unsigned int inf_count_intersection(segment seg, vector<line> bisector);

bool inf_query(point query_point, vector<vector<segment>> zone, point object, unsigned int k);

#endif