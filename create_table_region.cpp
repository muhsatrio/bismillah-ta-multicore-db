#include <iostream>
#include "lib/data.h"
#include "lib/segment.h"
#include <cmath>
#include <vector>
#include "mysql-cppconn-8/jdbc/mysql_connection.h"
#include "mysql-cppconn-8/jdbc/mysql_driver.h"
#include "mysql-cppconn-8/jdbc/cppconn/exception.h"
#include "mysql-cppconn-8/jdbc/cppconn/resultset.h"
#include "mysql-cppconn-8/jdbc/cppconn/statement.h"
#include "mysql-cppconn-8/jdbc/cppconn/prepared_statement.h"

using namespace std;

void create_table_region(int interest_point) {
    try {
            sql::Driver *driver;
            sql::Connection *con;
            sql::Statement *stat;

            driver = get_driver_instance();
            con = driver->connect(db_host, db_user, db_pass);
            con->setSchema(db_name);

            stat = con->createStatement();
            stat->execute("CREATE table region_" + to_string(interest_point) + "(id INT NOT NULL AUTO_INCREMENT, x DOUBLE, y DOUBLE, parallel_rank INT, PRIMARY KEY(id))");
            stat->execute("CREATE INDEX region_" + to_string(interest_point) + " ON region_" + to_string(interest_point) + "(id, x, y)");
            delete con;
            delete stat;
            // delete 
        }
        catch(sql::SQLException &e) {
            cout << "# ERR: SQLException in " << __FILE__;
            cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
            cout << "# ERR: " << e.what();
            cout << " (MySQL error code: " << e.getErrorCode();
            cout << ", SQLState: " << e.getSQLState() << " )" << endl;
        }
}

void create_table_result() {
    try {
            sql::Driver *driver;
            sql::Connection *con;
            sql::Statement *stat;

            driver = get_driver_instance();
            con = driver->connect(db_host, db_user, db_pass);
            con->setSchema(db_name);

            stat = con->createStatement();
            stat->execute("CREATE table result_region(id INT NOT NULL AUTO_INCREMENT, total_generate_point INT, total_parallel INT, time DOUBLE DEFAULT 0, PRIMARY KEY(id))");
            delete con;
            delete stat;
            // delete 
        }
        catch(sql::SQLException &e) {
            cout << "# ERR: SQLException in " << __FILE__;
            cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
            cout << "# ERR: " << e.what();
            cout << " (MySQL error code: " << e.getErrorCode();
            cout << ", SQLState: " << e.getSQLState() << " )" << endl;
        }
}

int main() {
    create_table_region(5);
    create_table_result();

}
