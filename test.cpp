#include "lib/point.h"
#include "lib/data.h"
#include "lib/vertex.h"
#include "lib/perpendicular_bisector.h"
#include <algorithm>
#include <iostream>
#include <cmath>

using namespace std;

// struct list_segment {
//     segment s[50000];
//     int size;
// };

segment segment_get_id(int interest_point, int id) {
    segment segment_result; 
        try {
            sql::Driver *driver;
            sql::Connection *con;
            // sql::Statement *stat;
            sql::ResultSet *res;
            sql::PreparedStatement *prep;

            driver = get_driver_instance();
            con = driver->connect(db_host, db_user, db_pass);
            con->setSchema(db_name);

            prep = con->prepareStatement("SELECT * FROM segment_" + to_string(interest_point) + " WHERE id=?");
            prep->setDouble(1, id);
            res = prep->executeQuery();
            while (res->next()) {
                segment_result.p1.x = res->getDouble("p1_x");
                segment_result.p1.y = res->getDouble("p1_y");
                segment_result.p2.x = res->getDouble("p2_x");
                segment_result.p2.y = res->getDouble("p2_y");
                segment_result.from = res->getInt("sisa_koneksi");
                segment_result.id = res->getInt("id");
            }   
            delete prep;
            delete con;
            delete res;
        }
        catch(sql::SQLException &e) {
            cout << "# ERR: SQLException in " << __FILE__;
            cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
            cout << "# ERR: " << e.what();
            cout << " (MySQL error code: " << e.getErrorCode();
            cout << ", SQLState: " << e.getSQLState() << " )" << endl;
        }
        return segment_result;
}

list_segment segment_get_related(int interest_point, point search_point, int id) {
    // vector<segment> segment_result;
    list_segment segment_result;
    segment_result.size = 0; 
        try {
            sql::Driver *driver;
            sql::Connection *con;
            // sql::Statement *stat;
            sql::ResultSet *res;
            sql::PreparedStatement *prep;

            driver = get_driver_instance();
            con = driver->connect(db_host, db_user, db_pass);
            con->setSchema(db_name);

            // prep = con->prepareStatement("SELECT * FROM segment_" + to_string(interest_point) + " WHERE ((p1_x=? AND p1_y=?) OR (p2_x=? AND p2_y=?))");
            prep = con->prepareStatement("SELECT * FROM segment_" + to_string(interest_point) + " WHERE ((ABS(p1_x-?)<0.00001 AND ABS(p1_y - ?) < 0.00001) OR (ABS(p2_x - ?) < 0.00001 AND ABS(p2_y - ?) < 0.00001))");
            prep->setDouble(1,search_point.x);
            prep->setDouble(2, search_point.y);
            prep->setDouble(3, search_point.x);
            prep->setDouble(4, search_point.y);
            res = prep->executeQuery();
            while (res->next()) {
                segment temp;
                temp.p1.x = res->getDouble("p1_x");
                temp.p1.y = res->getDouble("p1_y");
                temp.p2.x = res->getDouble("p2_x");
                temp.p2.y = res->getDouble("p2_y");
                temp.from = res->getInt("sisa_koneksi");
                temp.id = res->getInt("id");
                segment_result.s[segment_result.size] = temp;
                segment_result.size++;
            }   
            delete prep;
            delete con;
            delete res;
        }
        catch(sql::SQLException &e) {
            cout << "# ERR: SQLException in " << __FILE__;
            cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
            cout << "# ERR: " << e.what();
            cout << " (MySQL error code: " << e.getErrorCode();
            cout << ", SQLState: " << e.getSQLState() << " )" << endl;
        }
        return segment_result;
}

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
    // return a.x==b.x && a.y==b.y;
    return abs(a.x-b.x) < 0.00001 && abs(a.y-b.y) < 0.00001;
}

// bool is_same_segment(segment a, segment b) {
//     return ((a.p1.x==b.p1.x && a.p1.y==b.p1.y) || (a.p1.x==b.p2.x && a.p1.y==b.p2.y) || (a.p2.x==b.p1.x && a.p2.y==b.p1.y) || (a.p2.x==b.p2.x && a.p2.y==b.p2.y));
// }

list_segment find_region(int interest_point, int idx_search, int sisa_koneksi) {
    int count_ = 0;
    list_segment reg;
    reg.size = 0;
    // vector<segment> segment_related;
    list_segment segment_related;
    // segment_related.size = 0;
    segment temp_segment;
    temp_segment = segment_get_id(interest_point, idx_search);
    point start_point = temp_segment.p1;
    point begin_point = temp_segment.p1;
    point next_point = temp_segment.p2;
    reg.s[reg.size] = temp_segment;
    reg.size++;
    // region.push_back(temp_segment);
    int idx_min;
    bool found = true;
    while (!is_same_point(start_point, next_point) && found) {
        // cout << reg.size << endl;
        // cout << begin_point.x << ' ' << begin_point.y << endl;
        // cout << temp_segment.p1.x << ' '  << temp_segment.p1.y << " | "  << temp_segment.p2.x << ' '  << temp_segment.p2.y << ' ' << endl;
        idx_min = idx_search;
        int count = 0;
        double min_angle = 180, temp_angle;
        segment_related.size = 0;
        segment_related = segment_get_related(interest_point, next_point, idx_search);
        // cout << "dari: " << next_point.x << ' ' << next_point.y << endl;
        for (int i=0;i<segment_related.size;i++) {
            // cout << segment_related.s[i].p1.x << ' ' << segment_related.s[i].p1.y << ' ' << segment_related.s[i].p2.x << ' ' << segment_related.s[i].p2.y << endl;
            double vector_x1, vector_y1, vector_x2, vector_y2;
            if (segment_related.s[i].id!=idx_search && (is_same_point(next_point, segment_related.s[i].p1) || is_same_point(next_point, segment_related.s[i].p2))) {
                vector_x1 = begin_point.x - next_point.x;
                vector_y1 = begin_point.y - next_point.y;
                if (is_same_point(next_point, segment_related.s[i].p2)) {
                    temp_angle = get_angle(vector_x1, vector_y1, segment_related.s[i].p1.x - segment_related.s[i].p2.x, segment_related.s[i].p1.y - segment_related.s[i].p2.y);
                }
                else {
                    temp_angle = get_angle(vector_x1, vector_y1, segment_related.s[i].p2.x - segment_related.s[i].p1.x, segment_related.s[i].p2.y - segment_related.s[i].p1.y);
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
            segment s = segment_get_id(interest_point, segment_related.s[idx_min].id);
            if (sisa_koneksi==2) {
                if (s.from==2 || (s.from==1 && is_segment_bound(s))) {
                    if (is_same_point(next_point, segment_related.s[idx_min].p1)) {
                        next_point = segment_related.s[idx_min].p2;
                        begin_point = segment_related.s[idx_min].p1;
                    }
                    else {
                        next_point = segment_related.s[idx_min].p1;
                        begin_point = segment_related.s[idx_min].p2;
                    }
                    reg.s[reg.size] = segment_related.s[idx_min];
                    reg.size++;
                    idx_search = segment_related.s[idx_min].id;
                }
                else {
                    reg.size = 0;
                    found = false;
                }
            }
        }
        else {
            reg.size = 0;
        }
    }
    return reg;
}

// vector<segment> find_region(int interest_point, int idx_search, int sisa_koneksi) {
//     int count_ = 0;
//     vector<segment> region;
//     vector<segment> segment_related;
//     segment temp_segment;
//     temp_segment = segment_get_id(interest_point, idx_search);
//     point start_point = temp_segment.p1;
//     point begin_point = temp_segment.p1;
//     point next_point = temp_segment.p2;
//     region.push_back(temp_segment);
//     int idx_min;
//     bool found = true;
//     while (!is_same_point(start_point, next_point) && found) {
//         idx_min = idx_search;
//         int count = 0;
//         double min_angle = 180, temp_angle;
//         segment_related.clear();
//         segment_related = segment_get_related(interest_point, next_point, idx_search);
//         for (int i=0;i<segment_related.size();i++) {
//             double vector_x1, vector_y1, vector_x2, vector_y2;
//             if (segment_related[i].id!=idx_search && (is_same_point(next_point, segment_related[i].p1) || is_same_point(next_point, segment_related[i].p2))) {
//                 vector_x1 = begin_point.x - next_point.x;
//                 vector_y1 = begin_point.y - next_point.y;
//                 if (is_same_point(next_point, segment_related[i].p2)) {
//                     temp_angle = get_angle(vector_x1, vector_y1, segment_related[i].p1.x - segment_related[i].p2.x, segment_related[i].p1.y - segment_related[i].p2.y);
//                 }
//                 else {
//                     temp_angle = get_angle(vector_x1, vector_y1, segment_related[i].p2.x - segment_related[i].p1.x, segment_related[i].p2.y - segment_related[i].p1.y);
//                 }
//                 if (temp_angle>0 && temp_angle<min_angle) {
//                     min_angle = temp_angle;
//                     idx_min = i;
//                     count++;
//                 }
//             }
//         }
//         if (count==0)
//             found=false;
//         if (found) {
//             segment s = segment_get_id(interest_point, segment_related[idx_min].id);
//             if (sisa_koneksi==2) {
//                 if (s.from==2 || (s.from==1 && is_segment_bound(s))) {
//                     if (is_same_point(next_point, segment_related[idx_min].p1)) {
//                         next_point = segment_related[idx_min].p2;
//                         begin_point = segment_related[idx_min].p1;
//                     }
//                     else {
//                         next_point = segment_related[idx_min].p1;
//                         begin_point = segment_related[idx_min].p2;
//                     }
//                     region.push_back(segment_related[idx_min]);
//                     idx_search = segment_related[idx_min].id;
//                 }
//                 else {
//                     region.clear();
//                     found = false;
//                 }
//             }
//         }
//         else {
//             region.clear();
//         }
//     }
//     return region;
// }

int main() {
    for (int i=1;i<=89;i++) {
        // list_segment reg = find_region(5, 44, 2);
        list_segment reg = find_region(5, i, 2);
        cout << "id: " << i << " | size: " << reg.size << endl;
    }
    // cout << "===\n";
    // for (int i=0;i<reg.size;i++) {
    //     // cout << reg[i].p1.x << ' ' << reg[i].p1.y << " | " << reg[i].p2.x << ' ' << reg[i].p2.y << endl;
    //     cout << reg.s[i].p1.x << ' ' << reg.s[i].p1.y << " | " << reg.s[i].p2.x << ' ' << reg.s[i].p2.y << endl;
    // }
}