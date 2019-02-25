#ifndef DATA_H
#define DATA_H

#include "mpich/mpi.h"
#include <cstddef>

using namespace std;

struct point {
    double x;
    double y;
};

struct segment {
    point start;
    point end;
    unsigned int from;
};

struct line {
    double m;
    double c;
};

MPI_Datatype generateStructPoint();
MPI_Datatype generateStructLine();
MPI_Datatype generateStructSegment(MPI_Datatype struct_point);

#endif