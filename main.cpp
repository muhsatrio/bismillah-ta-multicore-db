#include <iostream>
#include "mpich/mpi.h"
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

struct point {
    double x;
    double y;
};

struct segment {
    point start;
    point end;
    unsigned int from;
};

struct line {
    double m;
    double c;
};

void point_insert(point interest_point, int id, int total_point) {
    try {
            sql::Driver *driver;
            sql::Connection *con;
            sql::Statement *stat;
            sql::PreparedStatement *prep;
            // sql::ResultSet *result;

            driver = get_driver_instance();
            con = driver->connect("localhost", "root", "");
            con->setSchema("bismillah_ta");
            string num_point_string = to_string(total_point);
            string num_id_string = to_string(id);
            stat = con->createStatement();
            prep = con->prepareStatement("INSERT INTO point_" + to_string(total_point) + "(id, x, y) VALUES(?, ?, ?)");
            prep->setInt(1, id);
            prep->setDouble(2, interest_point.x);
            prep->setDouble(3, interest_point.y);
            prep->execute();
            delete prep;
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

point rand_generate_point(double bound) {
    // cout << "randomize - generatePoint" << endl;
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    uniform_real_distribution<double> distribution(0.0, bound);
    return point {
        distribution(generator),
        distribution(generator)
    };
}

void point_generate(int num_point) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank==0) {
        for (int i=1;i<=num_point;i++) {
            point interest_point = rand_generate_point(100);
            point_insert(interest_point, i, num_point);
        }
    }
}

void point_init(int point) {
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
            stat->execute("CREATE table point_" + to_string(point) + "(id INT NOT NULL PRIMARY KEY, x DOUBLE, y DOUBLE)");
            stat->execute("CREATE INDEX idx_point_" + to_string(point) + " ON point_" + to_string(point) + "(x, y)");
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
        point_init(i*200);
        point_generate(i*200);
    }
    // cout << "hello\n";
    // cout << rank << endl;
    MPI_Finalize();
}