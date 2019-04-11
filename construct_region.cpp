#include <iostream>
#include "mpich/mpi.h"
#include "lib/data.h"
#include "lib/segment.h"
#include <cmath>
#include <vector>
#include "mysql-cppconn-8/jdbc/mysql_connection.h"
#include "mysql-cppconn-8/jdbc/mysql_driver.h"
#include "mysql-cppconn-8/jdbc/cppconn/exception.h"
#include "mysql-cppconn-8/jdbc/cppconn/resultset.h"
#include "mysql-cppconn-8/jdbc/cppconn/statement.h"
#include "mysql-cppconn-8/jdbc/cppconn/prepared_statement.h"
#include <ctime>

using namespace std;

double get_angle(double vector_x1, double vector_y1, double vector_x2, double vector_y2) {
    double dot = (vector_x1 * vector_x2) + (vector_y1 * vector_y2);
    double determinan = (vector_x1 * vector_y2) - (vector_x2 * vector_y1);
    return atan2(determinan, dot) * 180 / PI;
}

bool is_same_point(point a, point b) {
    return a.x==b.x && a.y==b.y;
}

bool is_same_segment(segment a, segment b) {
    return ((a.p1.x==b.p1.x && a.p1.y==b.p1.y) || (a.p1.x==b.p2.x && a.p1.y==b.p2.y) || (a.p2.x==b.p1.x && a.p2.y==b.p1.y) || (a.p2.x==b.p2.x && a.p2.y==b.p2.y));
}

void insert_region(int interest_point, point p) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    try {
            sql::Driver *driver;
            sql::Connection *con;
            sql::Statement *stat;
            sql::PreparedStatement *prep;
            // sql::ResultSet *result;

            driver = get_driver_instance();
            con = driver->connect(db_host, db_user, db_pass);
            con->setSchema(db_name);
            string num_point_string = to_string(interest_point);
            stat = con->createStatement();
            prep = con->prepareStatement("INSERT INTO region_" + to_string(interest_point) + "(x, y, parallel_rank) VALUES(?, ?, ?)");
            prep->setDouble(1, p.x);
            prep->setDouble(2, p.y);
            prep->setInt(3, rank);
            prep->execute();
            delete prep;
            delete con;
            delete stat;
            // delete 
        }
        catch(sql::SQLException &e) {
            cout << "# ERR: SQLException in " << __FILE__;
            cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
            cout << "# ERR: " << e.what();
            cout << " (MySQL error code: " << e.getErrorCode();
            cout << ", SQLState: " << e.getSQLState() << " )" << endl;
        }
}

// vector<segment> find_region(int idx_search, vector<segment> list_segment, int len_segment) {
//     vector<segment> region;
//     region.push_back(list_segment[idx_search]);
//     list_segment[idx_search].from--;
//     point start_point = list_segment[idx_search].p1;
//     point begin_point = list_segment[idx_search].p1;
//     point next_point=list_segment[idx_search].p2;
//     int idx_min;
//     bool found = true;
//     while (!is_same_point(start_point, next_point) && found) {
//         idx_min = idx_search;
//         int count = 0;
//         double min_angle = 180, temp_angle;
//         for (int i=0;i<len_segment;i++) {
//             double vector_x1, vector_y1, vector_x2, vector_y2;
//             if (i!=idx_search && (is_same_point(next_point, list_segment[i].p1) || is_same_point(next_point, list_segment[i].p2)) && list_segment[i].from>0) {
//                 count++;
//                 vector_x1 = begin_point.x - next_point.x;
//                 vector_y1 = begin_point.y - next_point.y;
//                 if (is_same_point(next_point, list_segment[i].p2)) {
//                     temp_angle = get_angle(list_segment[i].p1.x - list_segment[i].p2.x, list_segment[i].p1.y - list_segment[i].p2.y, vector_x1, vector_y1);
//                 }
//                 else {
//                     temp_angle = get_angle(list_segment[i].p2.x - list_segment[i].p1.x, list_segment[i].p2.y - list_segment[i].p1.y, vector_x1, vector_y1);
//                 }
//                 if ((temp_angle>0 && temp_angle<180) && temp_angle<min_angle) {
//                     min_angle = temp_angle;
//                     idx_min = i;
//                 }
//             }
//         }
//         if (count==0)
//             found=false;
//         if (found) {
//             idx_search = idx_min;
//             region.push_back(list_segment[idx_search]);
//             list_segment[idx_search].from--;
//             if (is_same_point(next_point, list_segment[idx_search].p1)) {
//                 next_point = list_segment[idx_search].p2;
//                 begin_point = list_segment[idx_search].p1;
//             }
//             else {
//                 next_point = list_segment[idx_search].p1;
//                 begin_point = list_segment[idx_search].p2;
//             }
//         }
//         count++;
//     }
//     return region;
// }

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // if (rank==0) {
    clock_t start;
    if (rank==0) {
        start = clock();
    }
    if (rank!=0) {
        insert_region(5, point{0, 0});
    }
    if (rank==0) {
        cout << ((double)clock()-start)/CLOCKS_PER_SEC;
    }
    // cout << "ok\n";
    // vector<segment> list_segment = segment_get(5, point{0, 0});
    // for (int i=0;i<list_segment.size();i++) {
    //     cout << list_segment[i].p1.x << ' ' << list_segment[i].p1.y << endl;
    //     cout << list_segment[i].p2.x << ' ' << list_segment[i].p2.y << endl;
    //     cout << "============\n";
    // }
    // }

    // init_table_parallel(5);
    // cout << rank << endl;
    MPI_Finalize();
}
