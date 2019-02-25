#ifndef POINT_H
#define POINT_H

#include "mpich/mpi.h"
#include "data.h"
#include "mysql-cppconn-8/jdbc/mysql_connection.h"
#include "mysql-cppconn-8/jdbc/mysql_driver.h"
#include "mysql-cppconn-8/jdbc/cppconn/exception.h"
#include "mysql-cppconn-8/jdbc/cppconn/resultset.h"
#include "mysql-cppconn-8/jdbc/cppconn/statement.h"
#include "mysql-cppconn-8/jdbc/cppconn/prepared_statement.h"
#include <string>
#include <chrono>
#include <random>

using namespace std;

void point_insert(point interest_point, int id, int total_point);
point point_rand_generate(double bound);
void point_generate(int num_point);
void point_init(int point);

#endif