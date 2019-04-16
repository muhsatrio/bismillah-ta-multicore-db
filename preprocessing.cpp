#include <iostream>
#include "lib/data.h"
#include "lib/point.h"
#include "lib/output.h"
#include "lib/perpendicular_bisector.h"
#include "lib/vertex.h"
#include "lib/segment.h"
#include <ctime>
#include <chrono>

using namespace std;

int main() {
    int multiplication_value = 5;
    int multiplication_times = 1;
    output_init();
    for (int i=1;i<=multiplication_times;i++) {
        point_init(i*multiplication_value);
        auto start_point = chrono::system_clock::now();
        point_generate(i*multiplication_value);
        chrono::duration<double> time_point = chrono::system_clock::now() - start_point;
        perpendicular_bisector_init(i*multiplication_value);
        auto start_perpendicular = chrono::system_clock::now();
        int size_perpendicular_bisector = perpendicular_bisector_generate(i*multiplication_value);
        chrono::duration<double> time_perpendicular_bisector = chrono::system_clock::now() - start_perpendicular;
        vertex_init(i*multiplication_value);
        auto start_vertex = chrono::system_clock::now();
        vertex_generate(i*multiplication_value, size_perpendicular_bisector);
        chrono::duration<double> time_vertex = chrono::system_clock::now() - start_vertex;
        segment_init(i*multiplication_value);
        auto start_segment = chrono::system_clock::now();
        segment_generate(i*multiplication_value, size_perpendicular_bisector);
        chrono::duration<double> time_segment = chrono::system_clock::now() - start_segment;
        double time_total = time_point.count() + time_perpendicular_bisector.count() + time_vertex.count() + time_segment.count();
        output_insert(i*multiplication_value, time_point.count(), time_perpendicular_bisector.count(), time_vertex.count(), time_segment.count(), time_total);
    }   
    // cout << "hello\n";
    // cout << rank << endl;
}
