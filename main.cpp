#include <iostream>
#include "mpich/mpi.h"
#include "mysql-cppconn-8/jdbc/mysql_connection.h"
#include "mysql-cppconn-8/jdbc/mysql_driver.h"
#include "mysql-cppconn-8/jdbc/cppconn/exception.h"
#include "mysql-cppconn-8/jdbc/cppconn/resultset.h"
#include "mysql-cppconn-8/jdbc/cppconn/statement.h"
#include <string>

using namespace std;

void database_point_init(int point) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank==0) {
        try {
            sql::Driver *driver;
            sql::Connection *con;
            sql::Statement *stat;
            // sql::ResultSet *result;

            driver = get_driver_instance();
            con = driver->connect("localhost", "root", "");
            con->setSchema("bismillah_ta");

            stat = con->createStatement();
            stat->execute("CREATE table point_" + to_string(point) + "(id INT, label CHAR(5))");
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
    // cout << "database_point_init\n";
}

int main(int argc, char *argv[]) {
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    for (int i=1;i<=5;i++) {
        database_point_init(i*20);
    }
    // cout << "hello\n";
    // cout << rank << endl;
    MPI_Finalize();
}