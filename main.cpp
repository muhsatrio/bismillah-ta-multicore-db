#include <iostream>
#include "mpich/mpi.h"
#include "lib/data.h"
#include "lib/point.h"
#include "lib/output.h"
#include <ctime>

using namespace std;

int main(int argc, char *argv[]) {
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int multiplication_value = 1000;
    clock_t time_point, time_total, start;
    output_init();
    for (int i=1;i<=5;i++) {
        point_init(i*multiplication_value);
        start = clock();
        point_generate(i*multiplication_value);
        time_point = clock() - start;
        time_total = time_point;
        output_insert(i*multiplication_value, ((double)time_point)/CLOCKS_PER_SEC, ((double)time_total)/CLOCKS_PER_SEC);
    }   
    // cout << "hello\n";
    // cout << rank << endl;
    MPI_Finalize();
}