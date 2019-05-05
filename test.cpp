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

void comparison_double(double a, double b) {
    if (a>b) {
        cout << "a lebih besar dari b" << endl;
    }
    else if (b>a) {
        cout << "b lebih besar dari a" << endl; 
    }
}

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

vector<segment> find_region(int interest_point, int idx_search) {
    int count_ = 0;
    vector<segment> region;
    vector<segment> segment_related;
    segment temp_segment;
    temp_segment = segment_get_id(interest_point, idx_search);
    point start_point = temp_segment.p1;
    point begin_point = temp_segment.p1;
    point next_point = temp_segment.p2;
    region.push_back(temp_segment);
    // region.push_back(start_point);
    // region.push_back(next_point);
    int idx_min;
    bool found = true;
    while (!is_same_point(start_point, next_point) && found) {
        idx_min = idx_search;
        int count = 0;
        double min_angle = 180, temp_angle;
        segment_related.clear();
        segment_related = segment_get_related(interest_point, next_point, idx_search);
        for (int i=0;i<segment_related.size();i++) {
            double vector_x1, vector_y1, vector_x2, vector_y2;
            if (segment_related[i].id!=idx_search && (is_same_point(next_point, segment_related[i].p1) || is_same_point(next_point, segment_related[i].p2)) && segment_related[i].from>0) {
                vector_x1 = begin_point.x - next_point.x;
                vector_y1 = begin_point.y - next_point.y;
                if (is_same_point(next_point, segment_related[i].p2)) {
                    temp_angle = get_angle(vector_x1, vector_y1, segment_related[i].p1.x - segment_related[i].p2.x, segment_related[i].p1.y - segment_related[i].p2.y);
                }
                else {
                    temp_angle = get_angle(vector_x1, vector_y1, segment_related[i].p2.x - segment_related[i].p1.x, segment_related[i].p2.y - segment_related[i].p1.y);
                }
                if (temp_angle>0 && temp_angle<min_angle) {
                    min_angle = temp_angle;
                    idx_min = i;
                    count++;
                }
            }
        }
        if (count==0)
            found=false;
        if (found) {
            if (is_same_point(next_point, segment_related[idx_min].p1)) {
                next_point = segment_related[idx_min].p2;
                begin_point = segment_related[idx_min].p1;
                // if (!is_same_point(next_point, start_point))
                //     region.push_back(next_point);
                    
            }
            else {
                next_point = segment_related[idx_min].p1;
                begin_point = segment_related[idx_min].p2;
                // if (!is_same_point(next_point, start_point))
                //     region.push_back(next_point);
            }
            region.push_back(segment_related[idx_min]);
            idx_search = segment_related[idx_min].id;
        }
        else {
            region.clear();
        }
    }
    return region;
}

int main() {
    double a = 5.000000000000001;
    double b = 5.000000000000001;
    comparison_double(a,b);
    // vector<segment> region = find_region(5, 25);
    // cout << region.size() << endl;
    // point temp_point;
    // for (int i=0;i<region.size();i++) {
    //     if (i==0) {
    //         temp_point = region[i].p2;
    //         cout << region[i].p1.x << ' ' << region[i].p1.y << endl;
    //     }
    //     else if (is_same_point(region[i].p1, temp_point)) {
    //         cout << region[i].p1.x << ' ' << region[i].p1.y << endl;
    //         temp_point = region[i].p2;
    //     }
    //     else {
    //         cout << region[i].p2.x << ' ' << region[i].p2.y << endl;
    //         temp_point = region[i].p1;
    //     }
    // }
}