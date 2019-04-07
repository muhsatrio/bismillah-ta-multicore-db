#include "perpendicular_bisector.h"

int perpendicular_bisector_generate(int interest_points) {
    int total;
    total = 0;
        for (int i=1;i<interest_points;i++) {
            for (int j=i+1;j<=interest_points;j++) {
                point point_1 = point_get(interest_points, i);
                point point_2 = point_get(interest_points, j);
                line perpendicular_bisector_gen = perpendicular_bisector_compute(point_1, point_2);
                perpendicular_bisector_insert(interest_points, perpendicular_bisector_gen, i, j);
                total++;
            }
        }
    return total;   
}

void perpendicular_bisector_insert(int interest_points, line perpendicular_bisector_gen, int id_point_1, int id_point_2) {
    try {
            sql::Driver *driver;
            sql::Connection *con;
            sql::Statement *stat;
            sql::PreparedStatement *prep;
            // sql::ResultSet *result;

            driver = get_driver_instance();
            con = driver->connect(db_host, db_user, db_pass);
            con->setSchema(db_name);
            string num_point_string = to_string(interest_points);
            stat = con->createStatement();
            prep = con->prepareStatement("INSERT INTO perpendicular_bisector_" + to_string(interest_points) + "(m, c, point_1, point_2) VALUES(?, ?, ?, ?)");
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
        try {
            sql::Driver *driver;
            sql::Connection *con;
            sql::Statement *stat;
            // sql::ResultSet *result;

            driver = get_driver_instance();
            con = driver->connect(db_host, db_user, db_pass);
            con->setSchema(db_name);

            stat = con->createStatement();
            stat->execute("CREATE table perpendicular_bisector_" + to_string(point) + "(id INT NOT NULL AUTO_INCREMENT, m DOUBLE, c DOUBLE, point_1 INT, point_2 INT, PRIMARY KEY(id))");
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

line perpendicular_bisector_get(int interest_point, int id) {
        line perpendicular_bisector_result;
        try {
            sql::Driver *driver;
            sql::Connection *con;
            sql::Statement *stat;
            sql::ResultSet *res;

            driver = get_driver_instance();
            con = driver->connect(db_host, db_user, db_pass);
            con->setSchema(db_name);

            stat = con->createStatement();
            res = stat->executeQuery("SELECT * FROM perpendicular_bisector_" + to_string(interest_point) + " WHERE id=" + to_string(id));
            while (res->next()) {
                // perpendicular_bisector_result.id = res->getInt("id");
                perpendicular_bisector_result.c = res->getDouble("c");
                perpendicular_bisector_result.m = res->getDouble("m");
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
        return perpendicular_bisector_result;
}