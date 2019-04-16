#ifndef DATA_H
#define DATA_H

#include <cstddef>
#include <string>

using namespace std;

struct point {
    double x;
    double y;
};

struct segment {
    point p1;
    point p2;
    int from;
    bool is_connected_p1_p2;
    bool is_connected_p2_p1;
    int id;
};

struct line {
    double m;
    double c;
};

const string db_host = "localhost";
const string db_name = "bismillah_ta";
const string db_user = "root";
const string db_pass = "";
const double bound = 100;
const double PI = 3.14159265;

#endif