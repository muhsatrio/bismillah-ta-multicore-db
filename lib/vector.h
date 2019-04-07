#ifndef VECTOR_H
#define VECTOR_H

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

void vector_init(int point);
void vector_generate(int interest_point, int size_perpendicular_bisector);
void vector_insert(int interest_points, vect vector_obj);
vector<vect> vector_create(int interest_point, int id_perpendicular);

#endif