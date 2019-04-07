#include "lib/point.h"
#include "lib/data.h"
#include "lib/vertex.h"
#include "lib/perpendicular_bisector.h"
#include "lib/vector.h"
#include "lib/region.h"
#include "mpich/mpi.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>

using namespace std;

// double get_angle(double vector_x1, double vector_y1, double vector_x2, double vector_y2) {
//     double dot = (vector_x1 * vector_x2) + (vector_y1 * vector_y2);
//     double determinan = (vector_x1 * vector_y2) - (vector_x2 * vector_y1);
//     return atan2(determinan, dot) * 180 / PI;
// }

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

        // cout << region_get_vector_angle(0, -2, 0, 2) << endl;
        line Line = line{2, 3};
        vector<point> Point = vertex_compute_bound(Line);
        for (int i=0;i<Point.size();i++) {
            cout << Point[i].x << ' ' << Point[i].y << endl;
        }

        // vector_init(5);
        // vector<vect> vector_obj = vector_create(5, 10);
        // cout << vector_obj.size() << endl;
        // for (int i=0;i<vector_obj.size();i++) {
        //     cout << vector_obj[i].start.x << ' ' << vector_obj[i].start.y << endl;
        //     cout << vector_obj[i].end.x << ' ' << vector_obj[i].end.y << endl;
        //     cout << "===============\n";
        // }

        // cout << result << endl;
        // line line_1 = line{5, 2};
        // line line_2 = line{10, 3};
        // point result = int_compute(line_2, line_1);
        // cout << result.x << ' ' << result.y << endl;
    }
    MPI_Finalize();
}