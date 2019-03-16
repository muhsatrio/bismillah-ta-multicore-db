#include "point.h"

void point_insert(point interest_point, int id, int total_point) {
    try {
            sql::Driver *driver;
            sql::Connection *con;
            sql::Statement *stat;
            sql::PreparedStatement *prep;
            // sql::ResultSet *result;

            driver = get_driver_instance();
            con = driver->connect(db_host, db_user, db_pass);
            con->setSchema(db_name);
            string num_point_string = to_string(total_point);
            string num_id_string = to_string(id);
            stat = con->createStatement();
            prep = con->prepareStatement("INSERT INTO point_" + to_string(total_point) + "(x, y) VALUES(?, ?)");
            // prep->setInt(1, id);
            prep->setDouble(1, interest_point.x);
            prep->setDouble(2, interest_point.y);
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

point point_rand_generate(double bound) {
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
            point interest_point = point_rand_generate(bound);
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
            con = driver->connect(db_host, db_user, db_pass);
            con->setSchema(db_name);

            stat = con->createStatement();
            stat->execute("CREATE table point_" + to_string(point) + "(id INT NOT NULL AUTO_INCREMENT, x DOUBLE, y DOUBLE, PRIMARY KEY(id))");
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
}

point point_get(int interest_point, int id) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank==0) {
        point point_result;
        try {
            sql::Driver *driver;
            sql::Connection *con;
            sql::Statement *stat;
            sql::ResultSet *res;

            driver = get_driver_instance();
            con = driver->connect(db_host, db_user, db_pass);
            con->setSchema(db_name);

            stat = con->createStatement();
            res = stat->executeQuery("SELECT * FROM point_" + to_string(interest_point) + " WHERE id=" + to_string(id));
            while (res->next()) {
                point_result.x = res->getDouble("x");
                point_result.y = res->getDouble("y");
            }   
            delete con;
            delete stat;
            delete res;
        }
        catch(sql::SQLException &e) {
            cout << "# ERR: SQLException in " << __FILE__;
            cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
            cout << "# ERR: " << e.what();
            cout << " (MySQL error code: " << e.getErrorCode();
            cout << ", SQLState: " << e.getSQLState() << " )" << endl;
        }
        return point_result;
    }
}