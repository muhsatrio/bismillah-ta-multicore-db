#include "perpendicular_bisector.h"

void perpendicular_bisector_generate(int interest_points) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank==0) {
        for (int i=1;i<interest_points;i++) {
            for (int j=i+1;j<=interest_points;j++) {
                point point_1 = point_get(interest_points, i);
                point point_2 = point_get(interest_points, j);
                line perpendicular_bisector_gen = perpendicular_bisector_compute(point_1, point_2);
                perpendicular_bisector_insert(interest_points, perpendicular_bisector_gen, i, j);
            }
        }
    }   
}

void perpendicular_bisector_insert(int interest_points, line perpendicular_bisector_gen, int id_point_1, int id_point_2) {
    try {
            sql::Driver *driver;
            sql::Connection *con;
            sql::Statement *stat;
            sql::PreparedStatement *prep;
            // sql::ResultSet *result;

            driver = get_driver_instance();
            con = driver->connect("localhost", "root", "");
            con->setSchema("bismillah_ta");
            string num_point_string = to_string(interest_points);
            stat = con->createStatement();
            prep = con->prepareStatement("INSERT INTO perpendicular_bisector_" + to_string(interest_points) + "(m, c, point_x, point_y) VALUES(?, ?, ?, ?)");
            prep->setDouble(1, perpendicular_bisector_gen.m);
            prep->setDouble(2, perpendicular_bisector_gen.c);
            prep->setInt(3, id_point_1);
            prep->setInt(4, id_point_2);
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

point perpendicular_bisector_calculate_mid(point point_1, point point_2) {
    return point{
        (point_1.x + point_2.x)/2,
        (point_1.y + point_2.y)/2
    };
}

double perpendicular_bisector_calculate_c(double slope, point mid_point) {
    return mid_point.y - (mid_point.x * slope); 
}

line perpendicular_bisector_compute(point point_1, point point_2) {
    double m = -1.0 / perpendicular_bisector_calculate_slope(point_1, point_2);
    double c = perpendicular_bisector_calculate_c(m, perpendicular_bisector_calculate_mid(point_1, point_2));
    return line{m, c};
}

double perpendicular_bisector_calculate_slope(point point_1, point point_2) {
    return (point_2.y - point_1.y) / (point_2.x - point_1.x);
}

void perpendicular_bisector_init(int point) {
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
            stat->execute("CREATE table perpendicular_bisector_" + to_string(point) + "(id INT NOT NULL AUTO_INCREMENT, m DOUBLE, c DOUBLE, point_x INT, point_y INT, PRIMARY KEY(id))");
            stat->execute("CREATE INDEX idx_perpendicular_bisector_" + to_string(point) + " ON perpendicular_bisector_" + to_string(point) + "(m, c)");
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