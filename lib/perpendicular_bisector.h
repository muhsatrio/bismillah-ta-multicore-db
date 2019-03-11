#ifndef PERPENDICULAR_BISECTOR_H
#define PERPENDICULAR_BISECTOR_H

#include "mpich/mpi.h"
#include "data.h"
#include "point.h"
#include "mysql-cppconn-8/jdbc/mysql_connection.h"
#include "mysql-cppconn-8/jdbc/mysql_driver.h"
#include "mysql-cppconn-8/jdbc/cppconn/exception.h"
#include "mysql-cppconn-8/jdbc/cppconn/resultset.h"
#include "mysql-cppconn-8/jdbc/cppconn/statement.h"
#include "mysql-cppconn-8/jdbc/cppconn/prepared_statement.h"
#include <string>

using namespace std;

void perpendicular_bisector_generate(int num_points);
void perpendicular_bisector_insert(int interest_points, line perpendicular_bisector_gen, int id_point_1, int id_point_2);
point perpendicular_bisector_calculate_mid(point point_1, point point_2);
double perpendicular_bisector_calculate_c(double slope, point mid_point);
line perpendicular_bisector_compute(point point_1, point point_2);
double perpendicular_bisector_calculate_slope(point point_1, point point_2);
void perpendicular_bisector_init(int point);

#endif
