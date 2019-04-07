#ifndef DATA_H
#define DATA_H

#include "mpich/mpi.h"
#include <cstddef>
#include <string>

using namespace std;

struct point {
    double x;
    double y;
};

struct vect {
    point start;
    point end;
    unsigned int from;
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

MPI_Datatype generateStructPoint();
MPI_Datatype generateStructLine();
MPI_Datatype generateStructVect(MPI_Datatype struct_point);

#endif