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
            stat->execute("CREATE TABLE result(interest_point INT PRIMARY KEY, time_point DOUBLE DEFAULT 0, time_perpendicular_bisector DOUBLE DEFAULT 0, time_vertex DOUBLE DEFAULT 0, time_vector DOUBLE DEFAULT 0, time_influence_zone DOUBLE DEFAULT 0, time_total DOUBLE DEFAULT 0)");
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

void output_insert(int interest_point, double time_point, double time_perpendicular_bisector, double time_vertex, double time_vector, double time_total) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank==0) {
        try {
            sql::Driver *driver;
            sql::Connection *con;
            sql::Statement *stat;
            sql::PreparedStatement *prep;

            driver = get_driver_instance();
            con = driver->connect(db_host, db_user, db_pass);
            con->setSchema(db_name);
            stat = con->createStatement();
            prep = con->prepareStatement("INSERT INTO result(interest_point, time_point, time_perpendicular_bisector, time_vertex, time_vector, time_total) VALUES(?, ?, ?, ?, ?, ?)");
            prep->setInt(1, interest_point);
            prep->setDouble(2, time_point);
            prep->setDouble(3, time_perpendicular_bisector);
            prep->setDouble(4, time_vertex);
            prep->setDouble(5, time_vector);
            prep->setDouble(6, time_total);
            prep->execute();
            delete prep;
            delete con;
            delete stat;
            cout << "DONE in interest point: " << interest_point << endl;
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