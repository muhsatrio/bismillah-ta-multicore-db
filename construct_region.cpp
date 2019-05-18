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
    return abs(a.x-b.x) < 0.00001 && abs(a.y-b.y) < 0.00001;
}

bool is_segment_bound(segment s) {
    return (s.p1.x==0 && s.p2.x==0) || (s.p1.y==0 || s.p2.y==0) || (s.p1.x==bound && s.p2.x==bound) || (s.p1.y==bound || s.p2.y==bound);
}

bool is_same_segment(segment a, segment b) {
    return ((a.p1.x==b.p1.x && a.p1.y==b.p1.y) || (a.p1.x==b.p2.x && a.p1.y==b.p2.y) || (a.p2.x==b.p1.x && a.p2.y==b.p1.y) || (a.p2.x==b.p2.x && a.p2.y==b.p2.y));
}

void insert_region(int interest_point, point p, int rank, int idx_search) {
    try {
            sql::Driver *driver;
            sql::Connection *con;
            sql::Statement *stat;
            sql::PreparedStatement *prep;

            driver = get_driver_instance();
            con = driver->connect(db_host, db_user, db_pass);
            con->setSchema(db_name);
            string num_point_string = to_string(interest_point);
            stat = con->createStatement();
            prep = con->prepareStatement("INSERT INTO region_" + to_string(interest_point) + "(x, y, label_region, parallel_rank) VALUES(?, ?, ?, ?)");
            prep->setDouble(1, p.x);
            prep->setDouble(2, p.y);
            prep->setInt(3, idx_search);
            prep->setInt(4, rank);
            prep->execute();
            delete prep;
            delete con;
            delete stat; 
        }
        catch(sql::SQLException &e) {
            cout << "# ERR: SQLException in " << __FILE__;
            cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
            cout << "# ERR: " << e.what();
            cout << " (MySQL error code: " << e.getErrorCode();
            cout << ", SQLState: " << e.getSQLState() << " )" << endl;
        }
}

void insert_record(int interest_point, int id_segment, int id_parallel, int segment_size) {
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
            prep = con->prepareStatement("INSERT INTO record_region_" + to_string(interest_point) + "(id_segment, id_parallel, segment_size, time_created) VALUES(?, ?, ?, NOW())");
            prep->setDouble(1, id_segment);
            prep->setDouble(2, id_parallel);
            prep->setInt(3, segment_size);
            prep->execute();
            delete prep;
            delete con;
            delete stat;
        }
        catch(sql::SQLException &e) {
            cout << "# ERR: SQLException in " << __FILE__;
            cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
            cout << "# ERR: " << e.what();
            cout << " (MySQL error code: " << e.getErrorCode();
            cout << ", SQLState: " << e.getSQLState() << " )" << endl;
        }
}

vector<segment> find_region(int interest_point, int idx_search, int sisa_koneksi) {
    int count_ = 0;
    vector<segment> region;
    vector<segment> segment_related;
    segment temp_segment;
    temp_segment = segment_get_id(interest_point, idx_search);
    point start_point = temp_segment.p1;
    point begin_point = temp_segment.p1;
    point next_point = temp_segment.p2;
    region.push_back(temp_segment);
    int idx_min;
    bool found = true;
    while (!is_same_point(start_point, next_point) && found) {
        idx_min = idx_search;
        int count = 0;
        double min_angle = 180, temp_angle;
        segment_related.clear();
        segment_related = segment_get_related(interest_point, next_point, idx_search);
        for (int i=0;i<segment_related.size();i++) {
            double vector_x1, vector_y1, vector_x2, vector_y2;
            if (segment_related[i].id!=idx_search && (is_same_point(next_point, segment_related[i].p1) || is_same_point(next_point, segment_related[i].p2))) {
                vector_x1 = begin_point.x - next_point.x;
                vector_y1 = begin_point.y - next_point.y;
                if (is_same_point(next_point, segment_related[i].p2)) {
                    temp_angle = get_angle(vector_x1, vector_y1, segment_related[i].p1.x - segment_related[i].p2.x, segment_related[i].p1.y - segment_related[i].p2.y);
                }
                else {
                    temp_angle = get_angle(vector_x1, vector_y1, segment_related[i].p2.x - segment_related[i].p1.x, segment_related[i].p2.y - segment_related[i].p1.y);
                }
                if (temp_angle>0 && temp_angle<min_angle) {
                    min_angle = temp_angle;
                    idx_min = i;
                    count++;
                }
            }
        }
        if (count==0)
            found=false;
        if (found) {
            segment s = segment_get_id(interest_point, segment_related[idx_min].id);
            if (s.from>0) {
                if (is_same_point(next_point, segment_related[idx_min].p1)) {
                    next_point = segment_related[idx_min].p2;
                    begin_point = segment_related[idx_min].p1;
                }
                else {
                    next_point = segment_related[idx_min].p1;
                    begin_point = segment_related[idx_min].p2;
                }
                region.push_back(segment_related[idx_min]);
                idx_search = segment_related[idx_min].id;
            }
            else {
                region.clear();
                found = false;
            }
        }
        else {
            region.clear();
        }
    }
    return region;
}

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int interest_point = 5;
    int idx_search = rank+1;
    int label_region = rank+1;
    int total_segment = segment_size(interest_point);
    int sisa_koneksi = 2;
    bool search = true;
    while (segment_available_sisa_koneksi(interest_point,2)>0) {
        segment s = segment_get_id(interest_point, idx_search);
        if (s.from==2) {
            vector<segment> reg = find_region(interest_point, s.id, 2);
            insert_record(interest_point, s.id, rank, reg.size());
            if (reg.size()>0) {
                point temp_point;
                for (int i=0;i<reg.size();i++) {
                    segment_decrement_sisa_koneksi(interest_point, reg[i].id);
                    if (i==0) {
                        insert_region(interest_point, reg[i].p1, rank, idx_search);
                        temp_point = reg[i].p2;
                    }
                    else if (is_same_point(reg[i].p1, temp_point)) {
                        insert_region(interest_point, reg[i].p1, rank, idx_search);
                        temp_point = reg[i].p2;
                    }
                    else {
                        insert_region(interest_point, reg[i].p2, rank, idx_search);
                        temp_point = reg[i].p1;
                    }
                }
            }
        }
        if (idx_search+size>total_segment) {
            idx_search = rank+1;
        }
        else {
            idx_search+=size;
        }
    }

    idx_search = rank + 1;

    while (segment_available_sisa_koneksi(interest_point,1)>0) {
        segment s = segment_get_id(interest_point, idx_search);
        if (s.from==1) {
            vector<segment> reg = find_region(interest_point, s.id, 1);
            insert_record(interest_point, s.id, rank, reg.size());
            if (reg.size()>0) {
                point temp_point;
                for (int i=0;i<reg.size();i++) {
                    segment_decrement_sisa_koneksi(interest_point, reg[i].id);
                    if (i==0) {
                        insert_region(interest_point, reg[i].p1, rank, idx_search);
                        temp_point = reg[i].p2;
                    }
                    else if (is_same_point(reg[i].p1, temp_point)) {
                        insert_region(interest_point, reg[i].p1, rank, idx_search);
                        temp_point = reg[i].p2;
                    }
                    else {
                        insert_region(interest_point, reg[i].p2, rank, idx_search);
                        temp_point = reg[i].p1;
                    }
                }
            }
        }
        if (idx_search+size>total_segment) {
            idx_search = rank+1;
        }
        else {
            idx_search+=size;
        }
    }

    MPI_Finalize();
}
