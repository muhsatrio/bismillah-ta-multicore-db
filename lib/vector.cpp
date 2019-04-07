#include "vector.h"

void vector_init(int point) {
        try {
            sql::Driver *driver;
            sql::Connection *con;
            sql::Statement *stat;
            // sql::ResultSet *result;

            driver = get_driver_instance();
            con = driver->connect(db_host, db_user, db_pass);
            con->setSchema(db_name);

            stat = con->createStatement();
            stat->execute("CREATE table vector_" + to_string(point) + "(id INT NOT NULL AUTO_INCREMENT, start_x DOUBLE, start_y DOUBLE, finish_x DOUBLE, finish_y DOUBLE, id_perpendicular INT, PRIMARY KEY(id))");
            stat->execute("CREATE INDEX idx_vector_" + to_string(point) + " ON vector_" + to_string(point) + "(start_x, start_y, finish_x, finish_y)");
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

void vector_generate(int interest_point, int size_perpendicular_bisector) {
        for (int i=-3;i<=size_perpendicular_bisector;i++) {
            vector<vect> temp = vector_create(interest_point, i);
            for (int j=0;j<temp.size();j++) {
                vector_insert(interest_point, temp[j]);
            }
        }
}

void vector_insert(int interest_points, vect vector_obj) {
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
            prep = con->prepareStatement("INSERT INTO vector_" + to_string(interest_points) + "(start_x, start_y, finish_x, finish_y) VALUES(?, ?, ?, ?)");
            prep->setDouble(1, vector_obj.start.x);
            prep->setDouble(2, vector_obj.start.y);
            prep->setDouble(3, vector_obj.end.x);
            prep->setDouble(4, vector_obj.end.y);
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

vector<vect> vector_create(int interest_point, int id_perpendicular) {
        point obj_start, obj_end;
        // double x1, x2, y1, y2;
        vector<vect> vector_obj;
        // double y;
        int total, count;
        try {
            sql::Driver *driver;
            sql::Connection *con;
            sql::Statement *stat;
            sql::ResultSet *res;
            sql::PreparedStatement *prep;

            driver = get_driver_instance();
            con = driver->connect(db_host, db_user, db_pass);
            con->setSchema(db_name);

            stat = con->createStatement();
            prep = con->prepareStatement("SELECT * FROM relation_vertex_perpendicular_" + to_string(interest_point) + " WHERE id_perpendicular=? ORDER BY vertex_x ASC, vertex_y ASC");
            prep->setInt(1, id_perpendicular);
            res = prep->executeQuery();
            total = res->rowsCount();
            count = 1;
            while (res->next()) {
                if (count==1) {
                    // obj_end.x = 
                    obj_end.x = res->getDouble("vertex_x");
                    obj_end.y = res->getDouble("vertex_y"); 
                }
                else if (count<=total) {
                    obj_start.x = obj_end.x;
                    obj_start.y = obj_end.y;
                    obj_end.x = res->getDouble("vertex_x");
                    obj_end.y = res->getDouble("vertex_y");
                    vector_obj.push_back({obj_start, obj_end, 0});
                }
                count++;
            }   
            delete prep;
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
        // return vertex_id;
        return vector_obj;
}