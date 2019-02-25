#include "output.h"

void output_init() {
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
            stat->execute("CREATE TABLE result(interest_point INT PRIMARY KEY, time_point DOUBLE DEFAULT 0, time_perpendicular_bisector DOUBLE DEFAULT 0, time_segment DOUBLE DEFAULT 0, time_influence_zone DOUBLE DEFAULT 0, time_total DOUBLE DEFAULT 0)");
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
}

void output_insert(int interest_point, double time_point, double time_total) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank==0) {
        try {
            sql::Driver *driver;
            sql::Connection *con;
            sql::Statement *stat;
            sql::PreparedStatement *prep;
            // sql::ResultSet *result;

            driver = get_driver_instance();
            con = driver->connect("localhost", "root", "");
            con->setSchema("bismillah_ta");
            stat = con->createStatement();
            prep = con->prepareStatement("INSERT INTO result(interest_point, time_point, time_total) VALUES(?, ?, ?)");
            // prep->setInt(1, id);
            prep->setInt(1, interest_point);
            prep->setDouble(2, time_point);
            prep->setDouble(3, time_total);
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
}