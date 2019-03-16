#ifndef OUTPUT_H
#define OUTPUT_H

#include "mpich/mpi.h"
#include "mysql-cppconn-8/jdbc/mysql_connection.h"
#include "mysql-cppconn-8/jdbc/mysql_driver.h"
#include "mysql-cppconn-8/jdbc/cppconn/exception.h"
#include "mysql-cppconn-8/jdbc/cppconn/resultset.h"
#include "mysql-cppconn-8/jdbc/cppconn/statement.h"
#include "mysql-cppconn-8/jdbc/cppconn/prepared_statement.h"
#include <string>

using namespace std;

void output_init();
void output_insert(int interest_point, double time_point, double time_perpendicular_bisector, double time_vertex, double time_total);


#endif 
