#include <iostream>
#include "lib/data.h"
#include "lib/point.h"
#include "lib/output.h"
#include "lib/perpendicular_bisector.h"
#include "lib/vertex.h"
#include "lib/segment.h"
#include <ctime>

using namespace std;

int main() {
    int multiplication_value = 5;
    int multiplication_times = 1;
    clock_t time_point, time_perpendicular_bisector, time_vertex, time_total, time_segment, start;
    output_init();
    for (int i=1;i<=multiplication_times;i++) {
        point_init(i*multiplication_value);
        start = clock();
        point_generate(i*multiplication_value);
        time_point = clock() - start;
        perpendicular_bisector_init(i*multiplication_value);
        start = clock();
        int size_perpendicular_bisector = perpendicular_bisector_generate(i*multiplication_value);
        time_perpendicular_bisector = clock() - start;
        vertex_init(i*multiplication_value);
        start = clock();
        vertex_generate(i*multiplication_value, size_perpendicular_bisector);
        time_vertex = clock() - start;
        segment_init(i*multiplication_value);
        start = clock();
        segment_generate(i*multiplication_value, size_perpendicular_bisector);
        time_segment = clock() - start;
        time_total = time_point + time_perpendicular_bisector + time_vertex + time_segment;
        output_insert(i*multiplication_value, ((double)time_point)/CLOCKS_PER_SEC, ((double)time_perpendicular_bisector)/CLOCKS_PER_SEC, ((double)time_vertex)/CLOCKS_PER_SEC, ((double)time_segment)/CLOCKS_PER_SEC, ((double)time_total)/CLOCKS_PER_SEC);
    }   
    // cout << "hello\n";
    // cout << rank << endl;
}