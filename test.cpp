#include "lib/point.h"
#include "lib/data.h"
#include "lib/vertex.h"
#include "lib/segment.h"
#include "lib/perpendicular_bisector.h"
#include <algorithm>
#include <iostream>
#include <cmath>

using namespace std;

void create_table_region(int interest_point) {
    try {
            sql::Driver *driver;
            sql::Connection *con;
            sql::Statement *stat;

            driver = get_driver_instance();
            con = driver->connect(db_host, db_user, db_pass);
            con->setSchema(db_name);

            stat = con->createStatement();
            stat->execute("CREATE table region_" + to_string(interest_point) + "(id INT NOT NULL AUTO_INCREMENT, x DOUBLE, y DOUBLE, label_region INT, parallel_rank INT, PRIMARY KEY(id))");
            stat->execute("CREATE INDEX region_" + to_string(interest_point) + " ON region_" + to_string(interest_point) + "(id, x, y)");
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

void create_table_result() {
    try {
            sql::Driver *driver;
            sql::Connection *con;
            sql::Statement *stat;

            driver = get_driver_instance();
            con = driver->connect(db_host, db_user, db_pass);
            con->setSchema(db_name);

            stat = con->createStatement();
            stat->execute("CREATE table result_region(id INT NOT NULL AUTO_INCREMENT, total_generate_point INT, total_parallel INT, time DOUBLE DEFAULT 0, PRIMARY KEY(id))");
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

void create_table_record(int interest_point) {
    try {
            sql::Driver *driver;
            sql::Connection *con;
            sql::Statement *stat;

            driver = get_driver_instance();
            con = driver->connect(db_host, db_user, db_pass);
            con->setSchema(db_name);

            stat = con->createStatement();
            stat->execute("CREATE table record_region_" + to_string(interest_point) + "(id INT NOT NULL AUTO_INCREMENT, id_segment INT, id_parallel INT, segment_size INT, time_created TIMESTAMP, PRIMARY KEY(id))");
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

void insert_region(int interest_point, point p, int rank, int idx_search) {
    // int rank;
    // MPI_Comm_rank(MPI_COMM_WORLD, &rank);
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
            prep = con->prepareStatement("INSERT INTO region_" + to_string(interest_point) + "(x, y, label_region, parallel_rank) VALUES(?, ?, ?, ?)");
            prep->setDouble(1, p.x);
            prep->setDouble(2, p.y);
            prep->setInt(3, idx_search);
            prep->setInt(4, rank);
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

void insert_record(int interest_point, int id_segment, int id_parallel, int segment_size) {
    // int rank;
    // MPI_Comm_rank(MPI_COMM_WORLD, &rank);
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
            // prep->setInt(4, rank);
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

// struct list_segment {
//     segment s[50000];
//     int size;
// };

// segment segment_get_id(int interest_point, int id) {
//     segment segment_result; 
//         try {
//             sql::Driver *driver;
//             sql::Connection *con;
//             // sql::Statement *stat;
//             sql::ResultSet *res;
//             sql::PreparedStatement *prep;

//             driver = get_driver_instance();
//             con = driver->connect(db_host, db_user, db_pass);
//             con->setSchema(db_name);

//             prep = con->prepareStatement("SELECT * FROM segment_" + to_string(interest_point) + " WHERE id=?");
//             prep->setDouble(1, id);
//             res = prep->executeQuery();
//             while (res->next()) {
//                 segment_result.p1.x = res->getDouble("p1_x");
//                 segment_result.p1.y = res->getDouble("p1_y");
//                 segment_result.p2.x = res->getDouble("p2_x");
//                 segment_result.p2.y = res->getDouble("p2_y");
//                 segment_result.from = res->getInt("sisa_koneksi");
//                 segment_result.id = res->getInt("id");
//             }   
//             delete prep;
//             delete con;
//             delete res;
//         }
//         catch(sql::SQLException &e) {
//             cout << "# ERR: SQLException in " << __FILE__;
//             cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
//             cout << "# ERR: " << e.what();
//             cout << " (MySQL error code: " << e.getErrorCode();
//             cout << ", SQLState: " << e.getSQLState() << " )" << endl;
//         }
//         return segment_result;
// }

// vector<segment> segment_get_related(int interest_point, point search_point, int id) {
//     vector<segment> segment_result; 
//         try {
//             sql::Driver *driver;
//             sql::Connection *con;
//             // sql::Statement *stat;
//             sql::ResultSet *res;
//             sql::PreparedStatement *prep;

//             driver = get_driver_instance();
//             con = driver->connect(db_host, db_user, db_pass);
//             con->setSchema(db_name);

//             // prep = con->prepareStatement("SELECT * FROM segment_" + to_string(interest_point) + " WHERE ((p1_x=? AND p1_y=?) OR (p2_x=? AND p2_y=?))");
//             prep = con->prepareStatement("SELECT * FROM segment_" + to_string(interest_point) + " WHERE ((ABS(p1_x-?)<0.00001 AND ABS(p1_y - ?) < 0.00001) OR (ABS(p2_x - ?) < 0.00001 AND ABS(p2_y - ?) < 0.00001))");
//             prep->setDouble(1,search_point.x);
//             prep->setDouble(2, search_point.y);
//             prep->setDouble(3, search_point.x);
//             prep->setDouble(4, search_point.y);
//             res = prep->executeQuery();
//             while (res->next()) {
//                 segment temp;
//                 temp.p1.x = res->getDouble("p1_x");
//                 temp.p1.y = res->getDouble("p1_y");
//                 temp.p2.x = res->getDouble("p2_x");
//                 temp.p2.y = res->getDouble("p2_y");
//                 temp.from = res->getInt("sisa_koneksi");
//                 temp.id = res->getInt("id");
//                 segment_result.push_back(temp);
//                 // segment_result.s[segment_result.size] = temp;
//                 // segment_result.size++;
//             }   
//             delete prep;
//             delete con;
//             delete res;
//         }
//         catch(sql::SQLException &e) {
//             cout << "# ERR: SQLException in " << __FILE__;
//             cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
//             cout << "# ERR: " << e.what();
//             cout << " (MySQL error code: " << e.getErrorCode();
//             cout << ", SQLState: " << e.getSQLState() << " )" << endl;
//         }
//         return segment_result;
// }

void comparison_double(double a, double b) {
    if (a>b) {
        cout << "a lebih besar dari b" << endl;
    }
    else if (b>a) {
        cout << "b lebih besar dari a" << endl; 
    }
}

double get_angle(double vector_x1, double vector_y1, double vector_x2, double vector_y2) {
    double dot = (vector_x1 * vector_x2) + (vector_y1 * vector_y2);
    double determinan = (vector_x1 * vector_y2) - (vector_x2 * vector_y1);
    return atan2(determinan, dot) * 180 / PI;
}

bool is_segment_bound(segment s) {
    return (s.p1.x==0 && s.p2.x==0) || (s.p1.y==0 || s.p2.y==0) || (s.p1.x==bound && s.p2.x==bound) || (s.p1.y==bound || s.p2.y==bound);
}

bool is_same_point(point a, point b) {
    return abs(a.x-b.x) < 0.00001 && abs(a.y-b.y) < 0.00001;
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
        // cout <<"dari: "<< next_point.x << ' ' << next_point.y << endl;
        idx_min = idx_search;
        int count = 0;
        double min_angle = 180, temp_angle;
        segment_related.clear();
        segment_related = segment_get_related(interest_point, next_point, idx_search);
        for (int i=0;i<segment_related.size();i++) {
            // cout << segment_related[i].p1.x << ' ' << segment_related[i].p1.y << ' ' << segment_related[i].p2.x << ' ' << segment_related[i].p2.y << ' ' << segment_related[i].from << endl;
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
            if (sisa_koneksi==2) {
                if (s.from>0) {
                // if (s.from==2 || (s.from==1 && is_segment_bound(s))) { //terduga bug
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
        }
        else {
            region.clear();
        }
    }
    // cout << "last point: " << next_point.x << ' ' << next_point.y << endl;
    return region;
}

int main() {
    // vector<segment> reg = find_region(5, 37, 2);
    // cout << reg.size() << endl;
    // for (int i=1;i<=89;i++) {
    //     // list_segment reg = find_region(5, 44, 2);
    //     vector<segment> reg = find_region(5, i, 2);
    //     cout << "id: " << i << " | size: " << reg.size() << endl;
    // }

    create_table_region(5);
    create_table_result();
    create_table_record(5);
    
    int interest_point = 5;
    int idx_search = 1;
    int total_segment = segment_size(interest_point);
    int sisa_koneksi = 2;
    bool search = true;
    while (segment_available_sisa_koneksi(interest_point,2)>0) {
        segment s = segment_get_id(interest_point, idx_search);
        if (s.from==2) {
            vector<segment> reg = find_region(interest_point, s.id, 2);
            insert_record(interest_point, s.id, 0, reg.size());
            if (reg.size()>0) {
                point temp_point;
                for (int i=0;i<reg.size();i++) {
                    segment_decrement_sisa_koneksi(interest_point, reg[i].id);
                    if (i==0) {
                        insert_region(interest_point, reg[i].p1, 0, idx_search);
                        temp_point = reg[i].p2;
                    }
                    else if (is_same_point(reg[i].p1, temp_point)) {
                        insert_region(interest_point, reg[i].p1, 0, idx_search);
                        temp_point = reg[i].p2;
                    }
                    else {
                        insert_region(interest_point, reg[i].p2, 0, idx_search);
                        temp_point = reg[i].p1;
                    }
                }
            }
        }
        if (idx_search+1>total_segment) {
            idx_search = 1;
        }
        else {
            idx_search++;
        }
    }
}