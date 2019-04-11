#include "lib/point.h"
#include "lib/data.h"
#include "lib/vertex.h"
#include "lib/segment.h"
#include "lib/perpendicular_bisector.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>

using namespace std;

double get_angle(double vector_x1, double vector_y1, double vector_x2, double vector_y2) {
    double dot = (vector_x1 * vector_x2) + (vector_y1 * vector_y2);
    double determinan = (vector_x1 * vector_y2) - (vector_x2 * vector_y1);
    return atan2(determinan, dot) * 180 / PI;
}

bool is_same_point(point a, point b) {
    return a.x==b.x && a.y==b.y;
}

bool is_same_segment(segment a, segment b) {
    return ((a.p1.x==b.p1.x && a.p1.y==b.p1.y) || (a.p1.x==b.p2.x && a.p1.y==b.p2.y) || (a.p2.x==b.p1.x && a.p2.y==b.p1.y) || (a.p2.x==b.p2.x && a.p2.y==b.p2.y));
}

vector<segment> find_region(int idx_search, vector<segment> list_segment, int len_segment) {
    vector<segment> region;
    region.push_back(list_segment[idx_search]);
    list_segment[idx_search].from--;
    point start_point = list_segment[idx_search].p1;
    point begin_point = list_segment[idx_search].p1;
    point next_point=list_segment[idx_search].p2;
    int idx_min;
    bool found = true;
    while (!is_same_point(start_point, next_point) && found) {
        idx_min = idx_search;
        int count = 0;
        double min_angle = 180, temp_angle;
        for (int i=0;i<len_segment;i++) {
            double vector_x1, vector_y1, vector_x2, vector_y2;
            if (i!=idx_search && (is_same_point(next_point, list_segment[i].p1) || is_same_point(next_point, list_segment[i].p2)) && list_segment[i].from>0) {
                count++;
                vector_x1 = begin_point.x - next_point.x;
                vector_y1 = begin_point.y - next_point.y;
                if (is_same_point(next_point, list_segment[i].p2)) {
                    temp_angle = get_angle(list_segment[i].p1.x - list_segment[i].p2.x, list_segment[i].p1.y - list_segment[i].p2.y, vector_x1, vector_y1);
                }
                else {
                    temp_angle = get_angle(list_segment[i].p2.x - list_segment[i].p1.x, list_segment[i].p2.y - list_segment[i].p1.y, vector_x1, vector_y1);
                }
                if ((temp_angle>0 && temp_angle<180) && temp_angle<min_angle) {
                    min_angle = temp_angle;
                    idx_min = i;
                }
            }
        }
        if (count==0)
            found=false;
        if (found) {
            idx_search = idx_min;
            region.push_back(list_segment[idx_search]);
            list_segment[idx_search].from--;
            if (is_same_point(next_point, list_segment[idx_search].p1)) {
                next_point = list_segment[idx_search].p2;
                begin_point = list_segment[idx_search].p1;
            }
            else {
                next_point = list_segment[idx_search].p1;
                begin_point = list_segment[idx_search].p2;
            }
        }
        count++;
    }
    return region;
}

int main() {
    // vector
}