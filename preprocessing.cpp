#include <iostream>
#include "lib/data.h"
#include "lib/point.h"
#include "lib/output.h"
#include "lib/perpendicular_bisector.h"
#include "lib/vertex.h"
#include "lib/segment.h"
#include <ctime>
#include <chrono>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

int convert_string_int(string s) {
    int len = s.length() - 1, pengali = 1, result = 0;
    while (len>=0) {
        result+=(((int)s[len] - 48) * pengali);
        pengali*=10;
        len--;
    }
    return result;
}

int main() {
    ifstream file_txt("interest_point.txt");
    string temp;
    vector<int> interest_point;
    if (file_txt.is_open()) {
        while (getline(file_txt, temp)) {
            int val = convert_string_int(temp);
            interest_point.push_back(val);
        }
    }
    output_init();
    for (int i=0;i<interest_point.size();i++) {
        point_init(interest_point[i]);
        auto start_point = chrono::system_clock::now();
        point_generate(interest_point[i]);
        chrono::duration<double> time_point = chrono::system_clock::now() - start_point;
        perpendicular_bisector_init(interest_point[i]);
        auto start_perpendicular = chrono::system_clock::now();
        int size_perpendicular_bisector = perpendicular_bisector_generate(interest_point[i]);
        chrono::duration<double> time_perpendicular_bisector = chrono::system_clock::now() - start_perpendicular;
        vertex_init(interest_point[i]);
        auto start_vertex = chrono::system_clock::now();
        vertex_generate(interest_point[i], size_perpendicular_bisector);
        chrono::duration<double> time_vertex = chrono::system_clock::now() - start_vertex;
        segment_init(interest_point[i]);
        auto start_segment = chrono::system_clock::now();
        segment_generate(interest_point[i], size_perpendicular_bisector);
        chrono::duration<double> time_segment = chrono::system_clock::now() - start_segment;
        double time_total = time_point.count() + time_perpendicular_bisector.count() + time_vertex.count() + time_segment.count();
        output_insert(interest_point[i], time_point.count(), time_perpendicular_bisector.count(), time_vertex.count(), time_segment.count(), time_total);
    }   
    // cout << "hello\n";
    // cout << rank << endl;
}
