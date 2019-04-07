#ifndef VERTEX_H
#define VERTEX_H

#include "mpich/mpi.h"
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

void vertex_init(int point);
double vertex_calculate_x(line line_1, line line_2);
point vertex_compute(line perpendicular_1, line perpendicular_2);
void vertex_generate(int interest_point, int perpendicular_size);
void vertex_insert(int interest_points, point vertex_gen);
vector<point> vertex_compute_bound(line Line);
int vertex_search(int interest_point, point vertex_obj);
int vertex_relation_search(int interest_point, int id_vertex, int id_perpendicular);
void vertex_relation_insert(int interest_point, int id_vertex, double vertex_x, double vertex_y, int id_perpendicular);
int vertex_get_id(int interest_point, point vertex_search);

#endif