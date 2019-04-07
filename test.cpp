#include "lib/point.h"
#include "lib/data.h"
#include "lib/vertex.h"
#include "lib/perpendicular_bisector.h"
#include "lib/vector.h"
#include "lib/region.h"
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

bool is_same_segment(vect a, vect b) {
    return ((a.start.x==b.start.x && a.start.y==b.start.y) || (a.start.x==b.end.x && a.start.y==b.end.y) || (a.end.x==b.start.x && a.end.y==b.start.y) || (a.end.x==b.end.x && a.end.y==b.end.y));
}

vector<vect> find_region(int i, vector<vect> list_segment, int len_segment) {
    vect next_segment=vect{0, 0, 0};
    double maks_angle = 360, temp_angle;
    while (!is_same_segment(list_segment[i], next_segment)) {
        for (int j=0;j<len_segment;j++) {
            if (j!=i && is_same_segment(list_segment[i], list_segment[j])) {
                if (is_same_point(list_segment[i].start, list_segment[j].start)) {
                    temp_angle = get_angle(list_segment[i].start.x - list_segment[i].end.x, list_segment[i].start.y - list_segment[i].end.y, list_segment[j].end.x - list_segment[j].start.x, list_segment[j].end.y - list_segment[j].start.y);
                }
                else if (is_same_point(list_segment[i].start, list_segment[j].end)) {
                    temp_angle = get_angle(list_segment[i].start.x - list_segment[i].end.x, list_segment[i].start.y - list_segment[i].end.y, list_segment[j].end.x - list_segment[j].start.x, list_segment[j].end.y - list_segment[j].start.y);
                }
                else if (is_same_point(list_segment[i].start, list_segment[j].end)) {

                }
                else if (is_same_point(list_segment[i].start, list_segment[j].end)) {

                } 
                // if (get_angle(list_segment[j].))
            }
        }
    }
    
}

int main() {
    vector<vect> list_segment;
    point a = point{2, 2};
    point b = point{3, 3};
    point c = point{4, 2};
    point d = point{1, 3};
    list_segment.push_back(vect{a, b, 0});
    list_segment.push_back(vect{b, c, 0});
    list_segment.push_back(vect{c, d, 0});
    list_segment.push_back(vect{d, a, 0});
    // cout << get_angle(-2, -2, 2, 2) << endl;
}