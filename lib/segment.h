#ifndef SEGMENT_H
#define SEGMENT_H

#include "data.h"
#include "perpendicular_bisector.h"
#include "mysql-cppconn-8/jdbc/mysql_connection.h"
#include "mysql-cppconn-8/jdbc/mysql_driver.h"
#include "mysql-cppconn-8/jdbc/cppconn/exception.h"
#include "mysql-cppconn-8/jdbc/cppconn/resultset.h"
#include "mysql-cppconn-8/jdbc/cppconn/statement.h"
#include "mysql-cppconn-8/jdbc/cppconn/prepared_statement.h"
#include <string>
#include <iomanip>
#include <vector>
#include <iostream>

using namespace std;

void segment_init(int point);
void segment_generate(int interest_point, int size_perpendicular_bisector);
void segment_insert(int interest_points, segment segment_obj);
vector<segment> segment_create(int interest_point, int id_perpendicular);
vector<segment> segment_get_related(int interest_point, point search_point, int id);
void segment_decrement_sisa_koneksi(int interest_point, int id);
segment segment_get_id(int interest_point, int id);
int segment_size_available(int interest_point);
int segment_available_sisa_koneksi(int interest_point, int sisa_koneksi);
int segment_size(int interest_point);

#endif