#include <iostream>
#include "mpich/mpi.h"
#include "lib/data.h"
#include "lib/point.h"
#include "lib/output.h"
#include "lib/perpendicular_bisector.h"
#include <ctime>

using namespace std;

int main(int argc, char *argv[]) {
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int multiplication_value = 10;
    int multiplication_times = 5;
    clock_t time_point, time_perpendicular_bisector, time_total, start;
    output_init();
    for (int i=1;i<=multiplication_times;i++) {
        point_init(i*multiplication_value);
        start = clock();
        point_generate(i*multiplication_value);
        time_point = clock() - start;
        perpendicular_bisector_init(i*multiplication_value);
        start = clock();
        perpendicular_bisector_generate(i*multiplication_value);
        time_perpendicular_bisector = clock() - start;
        time_total = time_point + time_perpendicular_bisector;
        output_insert(i*multiplication_value, ((double)time_point)/CLOCKS_PER_SEC, ((double)time_perpendicular_bisector)/CLOCKS_PER_SEC, ((double)time_total)/CLOCKS_PER_SEC);
    }   
    // cout << "hello\n";
    // cout << rank << endl;
    MPI_Finalize();
}