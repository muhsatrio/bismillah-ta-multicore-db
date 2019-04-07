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

using namespace std;

void segment_init(int point);
void segment_generate(int interest_point, int size_perpendicular_bisector);
void segment_insert(int interest_points, segment segment_obj);
vector<segment> segment_create(int interest_point, int id_perpendicular);

#endif