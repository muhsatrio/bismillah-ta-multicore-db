#ifndef REGION_H
#define REGION_H

#include "data.h"
#include "perpendicular_bisector.h"
#include "mysql-cppconn-8/jdbc/mysql_connection.h"
#include "mysql-cppconn-8/jdbc/mysql_driver.h"
#include "mysql-cppconn-8/jdbc/cppconn/exception.h"
#include "mysql-cppconn-8/jdbc/cppconn/resultset.h"
#include "mysql-cppconn-8/jdbc/cppconn/statement.h"
#include "mysql-cppconn-8/jdbc/cppconn/prepared_statement.h"
#include <string>
#include <cmath>

double region_get_vector_angle(double vector_x1, double vector_y1, double vector_x2, double vector_y2);

#endif