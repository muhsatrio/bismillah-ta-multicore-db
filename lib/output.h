#ifndef OUTPUT_H
#define OUTPUT_H

#include <vector>
#include "data.h"
#include <tuple>
#include <ctime>
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>

using namespace std;

struct output
{
    point query_point;
    vector<point> interest_point;
    vector<vector<segment>> influence_zone;
    time_t duration; 
};

void out_create_file_time();
void out_save_time(unsigned int count, double duration);
void out_create_file_segment();
void out_save_segment_file(unsigned int interests, unsigned int average);
void out_save_query_time(unsigned int k, unsigned int objects, time_t duration);
void out_create_file_size();
void out_save_file_size(unsigned int k, unsigned int avg_segment);
void out_save_query_csv(unsigned int interest_points, point query_point);
void out_create_interest_csv(vector<point> interest_points);
void out_file_zone_csv(unsigned int interest_points, vector<tuple<segment, unsigned int>> labeled_segment);
void out_zone_csv(vector<vector<segment>> zone, unsigned int count);
void out_point_csv(vector<point> interest_points);

#endif