#include "lib/point.h"
#include "lib/data.h"
#include "lib/vertex.h"
#include "lib/perpendicular_bisector.h"
#include "mpich/mpi.h"
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

bool int_rules_sorting(point a, point b) {
    // cout << "intersection - rules_sorting" << endl;
    if (a.x!=b.x) {
        return a.x < b.x;
    }
    if (a.y!=b.y) {
        return a.y < b.y;
    }
}

vector<point> int_sorting(vector<point> points) {
    // cout << "intersection - sorting" << endl;
    vector<point> sorted_;
    sorted_ = points;
    sort(sorted_.begin(), sorted_.end(), int_rules_sorting);
    return sorted_;
}

vector<point> int_remove_outside_bound(vector<point> points, double bound) {
    vector<point> cleaned;

    // for (vector<point>::iterator point=points.begin(); point!=points.end(); ++point) {
    for (int i=0;i<points.size();i++) {
        if (points[i].x >= 0.0 && points[i].x <= bound && points[i].y >= 0.0 && points[i].y <= bound) {
            cleaned.push_back(points[i]);
        }
    }
    return cleaned;
}

vector<point> int_compute_bound(line line_, double bound) {
    vector<point> points;

    point point_ = point{
        0.0,
        line_.c
    };
    points.push_back(point_);

    point_ = point{
        bound,
        (line_.m * bound) + line_.c
    };
    points.push_back(point_);
    
    point_ = point{
        -line_.c / line_.m,
        0.0
    };
    points.push_back(point_);

    point_ = point{
        (bound - line_.c) / line_.m,
        bound
    };
    points.push_back(point_);
    
    return points;

}

double int_calculate_x(line line_1, line line_2) {
    // cout << "intersection - calculate_x" << endl;
    return (line_2.c - line_1.c) / (line_1.m - line_2.m);
}

point int_compute(line line_1, line line_2) {
    // cout << "intersection - i_compute" << endl;
    double x = int_calculate_x(line_1, line_2);
    double y = (line_1.m * x) + line_1.c;
    return point{
        x, y
    };
}

vector<vector<point>> int_generate(vector<line> lines, double bound) {
    // cout << "intersection - i_generate" << endl;
    unsigned int outer = 0;
    unsigned int inner;
    vector<vector<point>> points;
    vector<point> point_;
    vector<point> cleaned;
    while (outer < lines.size()) {
        inner = 0;
        point_.clear();
        cleaned.clear();

        while (inner < lines.size()) {
            if (inner != outer) {
                point_.push_back(int_compute(lines[outer], lines[inner]));
            }
            inner++;
        }
        vector<point> temp;
        temp = int_compute_bound(lines[outer],bound);
        point_.insert(point_.end(), temp.begin(), temp.end());
        temp = int_sorting(int_remove_outside_bound(point_, bound));
        cleaned.insert(cleaned.end(), temp.begin(), temp.end());
        points.push_back(cleaned);
        outer++;
    }
    return points;
}

vector<point> int_generate_partial(vector<line> lines, double bound, unsigned int index) {
    // cout << "intersection - generate_partial" << endl;
    vector<point> intersections;
    vector<point> temp;
    vector<point> result;
    int inner = 0;
    // for (vector<line>::iterator line_=lines.begin();line_!=lines.end();line_++) {
    for (int i=0;i<lines.size();i++) {
        if (inner!=index) {
            intersections.push_back(int_compute(lines[index], lines[i]));
            // intersections.push_back(intersection_compute(lines[index], *line_));    
        }
        inner++;
    }
    temp = int_compute_bound(lines[index], bound);
    intersections.insert(intersections.end(), temp.begin(), temp.end());
    result = int_sorting(int_remove_outside_bound(intersections, bound));
    return result;
};

bool int_line_segment(line line_, segment segment_) {
    double t = (line_.c - segment_.start.y + line_.m * segment_.start.x) / (segment_.end.y - segment_.start.y + line_.m * (segment_.start.x - segment_.end.x));
    return t>0.0 && t<1.0;
}
unsigned int int_calculate_orientation(point point_1, point point_2, point point_3) {
    double val = (point_2.y - point_1.y) * (point_3.x - point_2.x) - (point_2.x - point_1.x) * (point_3.y - point_2.y);
    if (val==0)
        return 0;
    else if (val>0)
        return 1;
    else
        return 2;
}


bool int_segment(segment segment_1, segment segment_2) {
    unsigned int orientation_1 = int_calculate_orientation(segment_1.start, segment_1.end, segment_2.start);
    unsigned int orientation_2 = int_calculate_orientation(segment_1.start, segment_1.end, segment_2.end);
    unsigned int orientation_3 = int_calculate_orientation(segment_2.start, segment_2.end, segment_1.start);
    unsigned int orientation_4 = int_calculate_orientation(segment_2.start, segment_2.end, segment_1.end);
    return orientation_1 != orientation_2 && orientation_3 != orientation_4;
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int rank;
    // line result;
    // point point_1, point_2;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank==0) {
        // int i=1;

        // while (i<10) {
        //     int j;
        //     j=i+1;
        //     while (j<=10) {
        //         point_1 = point_get(10, i);
        //         cout << "p" << i << ' ' << point_1.x << ' ' << point_1.y << endl; 
        //         point_2 = point_get(10, j);
        //         cout << "p" << j << ' ' << point_2.x << ' ' << point_2.y << endl;
        //         // result = pb_compute(point_1, point_2);
        //         // cout << result.c << ' ' << result.m << endl; 
        //         j++; 
        //     }
        //     i++;
        // }
        // point temp;
        // temp = point_get(10, 1);
        // cout << temp.x << ' ' << temp.y << endl;
        // perpendicular_bisector_init(1000);
        point a;
        a.x = 0.3984607530490602;
        a.y = 44.44593839105441;
        point b;
        b.x = 3;
        b.y = 6;
        vertex_relation_insert(10, 2, 2);
        // cout << result << endl;
        // line line_1 = line{5, 2};
        // line line_2 = line{10, 3};
        // point result = int_compute(line_2, line_1);
        // cout << result.x << ' ' << result.y << endl;
    }
    MPI_Finalize();
}