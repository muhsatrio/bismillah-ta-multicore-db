#include <iostream>
#include "lib/data.h"
#include "lib/segment.h"
#include <cmath>

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

void insert_region(int interest_point, point p, int rank, int idx_search, int order_val, int parallel_size) {
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
            prep = con->prepareStatement("INSERT INTO region_" + to_string(interest_point) +"_"+ to_string(parallel_size) + "(x, y, order_val, label_region, parallel_rank) VALUES(?, ?, ?, ?, ?)");
            prep->setDouble(1, p.x);
            prep->setDouble(2, p.y);
            prep->setInt(3, order_val);
            prep->setInt(4, idx_search);
            prep->setInt(5, rank);
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

void insert_record(int interest_point, int id_segment, int id_parallel, int segment_size, int parallel_size) {
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
            prep = con->prepareStatement("INSERT INTO record_region_" + to_string(interest_point) +"_"+ to_string(parallel_size) + "(id_segment, id_parallel, segment_size, time_created) VALUES(?, ?, ?, NOW())");
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

void insert_time_result(int interest_point, int parallel_size, double total_time) {
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
            prep = con->prepareStatement("INSERT INTO result_region(total_generate_point, total_parallel, time) VALUES(?, ?, ?)");
            prep->setInt(1, interest_point);
            prep->setInt(2, parallel_size);
            prep->setDouble(3, total_time);
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
    bool is_there_redudant_region = false;
    while (!is_same_point(start_point, next_point) && found) {
        if (segment_available_sisa_koneksi(interest_point, -1)>0) {
            is_there_redudant_region = true;
        }
        cout << "dari: " << next_point.x << ' ' << next_point.y << endl;
        idx_min = idx_search;
        int count = 0;
        double min_angle = 180, temp_angle;
        segment_related.clear();
        segment_related = segment_get_related(interest_point, next_point, idx_search);
        for (int i=0;i<segment_related.size();i++) {
            double vector_x1, vector_y1, vector_x2, vector_y2;
            cout << "===============\n";
            cout << segment_related[i].p1.x << ' ' << segment_related[i].p1.y << ' ' << segment_related[i].p2.x << ' ' << segment_related[i].p2.y << endl;
            cout << segment_related[i].from << endl;
            cout << segment_related[i].is_connected_p1_p2 << ' ' << segment_related[i].is_connected_p2_p1 << endl;
            cout << "===============\n";
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
                    if (s.is_connected_p1_p2==false) {
                        next_point = segment_related[idx_min].p2;
                        begin_point = segment_related[idx_min].p1;
                    }
                    else {
                        region.clear();
                        found=false;
                    }
                }
                else {
                    if (s.is_connected_p2_p1==false) {
                        next_point = segment_related[idx_min].p1;
                        begin_point = segment_related[idx_min].p2;
                    }
                    else {
                        region.clear();
                        found = false;
                    }
                }
                if (found) {
                    region.push_back(segment_related[idx_min]);
                    idx_search = segment_related[idx_min].id;
                }
            }
            else {
                if (is_there_redudant_region) {
                    if (is_same_point(next_point, segment_related[idx_min].p1)) {
                        // if (s.is_connected_p1_p2==false) {
                            next_point = segment_related[idx_min].p2;
                            begin_point = segment_related[idx_min].p1;
                        // }
                    }
                    else {
                        // if (s.is_connected_p2_p1==false) {
                            next_point = segment_related[idx_min].p1;
                            begin_point = segment_related[idx_min].p2;
                        // }
                    }
                    cout << "hihi\n";
                    region.push_back(segment_related[idx_min]);
                    idx_search = segment_related[idx_min].id;
                }
                else {
                    cout << "hehe\n";
                    region.clear();
                    found = false;
                }
            }
        }
        else {
            region.clear();
        }
    }
    cout << is_there_redudant_region << endl;
    return region;
}

int main() {
    vector<segment> region = find_region(5, 42, 2);
    cout << "size: " << region.size() << endl;
}