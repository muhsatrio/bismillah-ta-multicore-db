#include <iostream>
#include <vector>
#include "lib/data.h"
#include "lib/randomize.h"
#include "lib/segment.h"
#include "lib/intersection.h"
#include "lib/common.h"
#include "lib/perpendicular_bisector.h"
#include "lib/influence_zone.h"
#include "lib/output.h"
#include "mpich/mpi.h"
#include <ctime>

using namespace std;

vector<vector<segment>> compute(unsigned int count, point query_point) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // cout << count << " " << rank << endl;
    double bound = 10.0;
    vector<point> interest_points;
    point temp;
    for (int i=0;i<count;i++) {
        temp = rand_generate(bound);
        interest_points.push_back(temp);
    }
    clock_t start, duration;
    start = clock();
    
    vector<vector<segment>> zone = inf_compute(count, query_point, interest_points, bound);

    duration = clock() - start;

    // cout << "haha" << endl;
    if (rank==0)    {
        out_save_time(count, ((double)duration)/CLOCKS_PER_SEC);
        out_zone_csv(zone, count);
        // out_save_segment_file(count, average);
        // out_save_file_size(count, average);
    }

    return zone;
}
void compute_object_k(unsigned int data_count[8]) {
    double bound = 10.0;
    point query_point = rand_generate(bound);
    vector<vector<segment>> zone = compute(data_count[7], query_point);
    vector<point> objects;
    // time_t start, finish;
    // double duration;
    bool found;
    bool totally = false;
    clock_t start, duration;

    for (int i=0;i<8;i++) {
        objects.clear();

        for (int j=0;j<(data_count[i] + 1) * 1000;j++) {
            objects.push_back(rand_generate(bound));
        }
        duration = 0;
        for (int k=0;k<8;k++) {
            for (int l=0;l<objects.size();i++) {
                start = clock();
                found = inf_query(query_point, zone, objects[l], data_count[k]);
                // time(&finish);
                duration+=(clock() - start);
                totally = totally || found;
            }
            out_save_query_time(data_count[k], data_count[i] * 100, ((double)duration)/CLOCKS_PER_SEC);
        }
    }
}

void compute_k(unsigned int interest_points, unsigned int multiplier, unsigned int object) {
    double bound = 10.0;
    point query_point = rand_generate(bound);
    vector<vector<segment>> zone = compute(interest_points, query_point);
    vector<point> objects;
    clock_t start, duration;
    bool found;
    bool totally;
    totally = false;
    for (int k=0;k<(interest_points/multiplier + 1);k++) {
        objects.clear();
        for (int i=0;i<object;i++) {
            objects.push_back(rand_generate(bound));
        }
        duration = 0;
        for (int i=0;i<objects.size();i++) {
            start = clock();
            found = inf_query(query_point, zone, objects[i], k * multiplier);
            totally = totally || found;
            duration+=(clock() - start);
        }
        out_save_query_time(k * multiplier, objects.size(), ((double)duration)/CLOCKS_PER_SEC);
    }
}

void generate(unsigned int total) {
    point query_point;
    for (int count=2;count<total;count++) {
        query_point = rand_generate(10.0);
        compute(count, query_point);
    }
}

void generate_mul(int total, int multiplier) {
    point query_point;
    for (int count=1;count<=total;count++) {
        query_point = rand_generate(10.0);
        vector<vector<segment>> zone = compute(count * multiplier, query_point);
    }
}

void generate_mul_start(unsigned int start, unsigned int total, unsigned int multiplier) {
    point query_point;
    vector<vector<segment>> zone;
    for (int count=1;count<=total;count++) {
        query_point = rand_generate(10.0);
        zone = compute(count * multiplier + start, query_point);
    }
}

//Masih diperbaiki

void compute_partial(unsigned int count, point query_point) {
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double bound = 10.0;
    vector<point> interest_points;

    for (int i=0;i<count;i++) {
        interest_points.push_back(rand_generate(bound));
    }
    // for (int i=0;i<interest_points.size();i++) {
    //     cout << interest_points[i].x << ' ' << interest_points[i].y << endl;
    // }

    clock_t start, duration;
    start = clock();
    inf_compute_partial(query_point, interest_points, bound);
    duration = clock() - start;
    if (rank==0) {
        out_save_time(count, ((double)duration)/CLOCKS_PER_SEC);
    }
}

void generate_mul_start_csv(unsigned int start, unsigned int total, unsigned int multiplier) {
    point query_point;
    for (int count=1;count<=total;count++) {
        query_point = rand_generate(10);
        compute_partial(count * multiplier + start, query_point);
    }
}

int main(int argc, char *argv[])
{   
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // unsigned int data_count[8] = {1, 5, 10, 15, 20, 25, 30, 35};
    // compute_object_k(data_count);

    // cout << rank << endl;

    // if (rank==0) {
    //     out_create_file_segment();
    //     out_create_file_size();
    //     out_create_file_time();
    // }
    // generate_mul(2, 40);
    // generate_mul_start_csv(0, 2, 50);
    if (rank==0) {
        vector<point> interest_points;
        for (int i=0;i<interest_points.size();i++) {
            temp = rand_generate(bound);
            interest_points.push_back(temp);
        }
        out_point_csv(interests_points);
    }
    MPI_Finalize();    
    return 0;
}

// output::create_query_file();
    // let data_count: [usize; 8] = [1, 100, 500, 1000, 1500, 2000, 2500, 3000];
    // compute_object_k(&data_count);

    // output::create_time_file();
    // output::create_segment_file();
    // output::create_size_file();
    // generate_mul_start(100, 10, 100);

    // generate_mul(4, 500);

    // output::create_query_file();
    // compute_k(4000, 200, 10000);

    // generate_mul_start_csv(0, 10, 100);

    //simulasi bisa send dan receive

    // if (rank==0) {
    //     vector<segment> dataSegment;
    //     segment temp;
    //     for (int i=1;i<size;i++) {
    //         MPI_Recv(&temp, 1, struct_segment, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    //         dataSegment.push_back(temp);
    //     }
    //     for (int i=0;i<dataSegment.size();i++) {
    //         cout << dataSegment[i].start.x << ' ' << dataSegment[i].start.y << endl;
    //         cout << dataSegment[i].end.x << ' ' << dataSegment[i].end.y << endl;
    //         cout << "======" << endl;
    //     }
    // }
    // else {
    //     point x = point{(double)rank, (double)rank * 2};
    //     point x2 = point{(double)rank + 1, (double)rank * 2 + 1};
    //     segment y = segment{x, x2, 0};
    //     MPI_Send(&y, 1, struct_segment, 0, 1, MPI_COMM_WORLD);
    // }
