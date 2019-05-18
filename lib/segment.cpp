#include "segment.h"

void segment_init(int point) {
        try {
            sql::Driver *driver;
            sql::Connection *con;
            sql::Statement *stat;
            // sql::ResultSet *result;

            driver = get_driver_instance();
            con = driver->connect(db_host, db_user, db_pass);
            con->setSchema(db_name);

            stat = con->createStatement();
            stat->execute("CREATE table segment_" + to_string(point) + "(id INT NOT NULL AUTO_INCREMENT, p1_x DOUBLE, p1_y DOUBLE, p2_x DOUBLE, p2_y DOUBLE, sisa_koneksi INT, PRIMARY KEY(id))");
            stat->execute("CREATE INDEX idx_segment_" + to_string(point) + " ON segment_" + to_string(point) + "(p1_x, p1_y, p2_x, p2_y, sisa_koneksi)");
            delete con;
            delete stat; 
        }
        catch(sql::SQLException &e) {
            cout << "# ERR: SQLException in " << __FILE__;
            cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
            cout << "# ERR: " << e.what();
            cout << " (MySQL error code: " << e.getErrorCode();
            cout << ", SQLState: " << e.getSQLState() << " )" << endl;
        }
}

void segment_generate(int interest_point, int size_perpendicular_bisector) {
        for (int i=-3;i<=size_perpendicular_bisector;i++) {
            vector<segment> temp = segment_create(interest_point, i);
            for (int j=0;j<temp.size();j++) {
                if ((temp[j].p1.x==0 && temp[j].p2.x==0) || (temp[j].p1.y==0 && temp[j].p2.y==0) || (temp[j].p1.x==bound && temp[j].p2.x==bound) || (temp[j].p1.y==bound && temp[j].p2.y==bound)) {
                    temp[j].from = 1;
                }
                else {
                    temp[j].from = 2;
                }
                // double diffx = temp[j].p1.x - temp[j].p2.x;
                // double diffy = temp[j].p1.y - temp[j].p2.y;
                // if (diffx<0)
                //     diffx = diffx * (-1);
                // if (diffy<0)
                //     diffy = diffy * (-1);
                // if (diffx>0.000009 || diffy>0.000009)
                //     segment_insert(interest_point, temp[j]);
                if (abs(temp[j].p1.x - temp[j].p2.x)>0.000009 || abs(temp[j].p1.y - temp[j].p2.y)>0.000009) {
                    segment_insert(interest_point, temp[j]);
                }
            }
        }
}

void segment_insert(int interest_points, segment segment_obj) {
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
            prep = con->prepareStatement("INSERT INTO segment_" + to_string(interest_points) + "(p1_x, p1_y, p2_x, p2_y, sisa_koneksi) VALUES(?, ?, ?, ?, ?)");
            prep->setDouble(1, segment_obj.p1.x);
            prep->setDouble(2, segment_obj.p1.y);
            prep->setDouble(3, segment_obj.p2.x);
            prep->setDouble(4, segment_obj.p2.y);
            prep->setDouble(5, segment_obj.from);
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

vector<segment> segment_create(int interest_point, int id_perpendicular) {
        point obj_start, obj_end;
        vector<segment> vector_obj;
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
        return vector_obj;
}

vector<segment> segment_get_related(int interest_point, point search_point, int id) {
    vector<segment> segment_result; 
        try {
            sql::Driver *driver;
            sql::Connection *con;
            // sql::Statement *stat;
            sql::ResultSet *res;
            sql::PreparedStatement *prep;

            driver = get_driver_instance();
            con = driver->connect(db_host, db_user, db_pass);
            con->setSchema(db_name);

            // prep = con->prepareStatement("SELECT * FROM segment_" + to_string(interest_point) + " WHERE ((p1_x=? AND p1_y=?) OR (p2_x=? AND p2_y=?))");
            prep = con->prepareStatement("SELECT * FROM segment_" + to_string(interest_point) + " WHERE ((ABS(p1_x-?)<0.00001 AND ABS(p1_y - ?) < 0.00001) OR (ABS(p2_x - ?) < 0.00001 AND ABS(p2_y - ?) < 0.00001))");
            prep->setDouble(1,search_point.x);
            prep->setDouble(2, search_point.y);
            prep->setDouble(3, search_point.x);
            prep->setDouble(4, search_point.y);
            res = prep->executeQuery();
            while (res->next()) {
                segment temp;
                temp.p1.x = res->getDouble("p1_x");
                temp.p1.y = res->getDouble("p1_y");
                temp.p2.x = res->getDouble("p2_x");
                temp.p2.y = res->getDouble("p2_y");
                temp.from = res->getInt("sisa_koneksi");
                temp.id = res->getInt("id");
                segment_result.push_back(temp);
                // segment_result.s[segment_result.size] = temp;
                // segment_result.size++;
            }   
            delete prep;
            delete con;
            delete res;
        }
        catch(sql::SQLException &e) {
            cout << "# ERR: SQLException in " << __FILE__;
            cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
            cout << "# ERR: " << e.what();
            cout << " (MySQL error code: " << e.getErrorCode();
            cout << ", SQLState: " << e.getSQLState() << " )" << endl;
        }
        return segment_result;
}

void segment_decrement_sisa_koneksi(int interest_point, int id) {
    try {
            sql::Driver *driver;
            sql::Connection *con;
            sql::Statement *stat;
            sql::PreparedStatement *prep;
            // sql::ResultSet *result;

            driver = get_driver_instance();
            con = driver->connect(db_host, db_user, db_pass);
            con->setSchema(db_name);
            stat = con->createStatement();
            prep = con->prepareStatement("UPDATE segment_" + to_string(interest_point) + " SET sisa_koneksi = sisa_koneksi - 1 WHERE id=?");
            prep->setDouble(1, id);
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

segment segment_get_id(int interest_point, int id) {
    segment segment_result; 
        try {
            sql::Driver *driver;
            sql::Connection *con;
            // sql::Statement *stat;
            sql::ResultSet *res;
            sql::PreparedStatement *prep;

            driver = get_driver_instance();
            con = driver->connect(db_host, db_user, db_pass);
            con->setSchema(db_name);

            prep = con->prepareStatement("SELECT * FROM segment_" + to_string(interest_point) + " WHERE id=?");
            prep->setDouble(1, id);
            res = prep->executeQuery();
            while (res->next()) {
                segment_result.p1.x = res->getDouble("p1_x");
                segment_result.p1.y = res->getDouble("p1_y");
                segment_result.p2.x = res->getDouble("p2_x");
                segment_result.p2.y = res->getDouble("p2_y");
                segment_result.from = res->getInt("sisa_koneksi");
                segment_result.id = res->getInt("id");
            }   
            delete prep;
            delete con;
            delete res;
        }
        catch(sql::SQLException &e) {
            cout << "# ERR: SQLException in " << __FILE__;
            cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
            cout << "# ERR: " << e.what();
            cout << " (MySQL error code: " << e.getErrorCode();
            cout << ", SQLState: " << e.getSQLState() << " )" << endl;
        }
        return segment_result;
}

int segment_size_available(int interest_point) {
    int size_segment; 
        try {
            sql::Driver *driver;
            sql::Connection *con;
            sql::Statement *stat;
            sql::ResultSet *res;
            // sql::PreparedStatement *prep;

            driver = get_driver_instance();
            con = driver->connect(db_host, db_user, db_pass);
            con->setSchema(db_name);

            stat = con->createStatement();
            res = stat->executeQuery("SELECT COUNT(*) AS count_segment FROM segment_" + to_string(interest_point) + " WHERE sisa_koneksi>0");
            while (res->next()) {
                size_segment = res->getInt("count_segment");
            }
            delete stat;
            delete con;
            delete res;
        }
        catch(sql::SQLException &e) {
            cout << "# ERR: SQLException in " << __FILE__;
            cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
            cout << "# ERR: " << e.what();
            cout << " (MySQL error code: " << e.getErrorCode();
            cout << ", SQLState: " << e.getSQLState() << " )" << endl;
        }
        return size_segment;
}

int segment_available_sisa_koneksi(int interest_point, int sisa_koneksi) {
    int size_segment; 
        try {
            sql::Driver *driver;
            sql::Connection *con;
            // sql::Statement *stat;
            sql::ResultSet *res;
            sql::PreparedStatement *prep;

            driver = get_driver_instance();
            con = driver->connect(db_host, db_user, db_pass);
            con->setSchema(db_name);

            prep = con->prepareStatement("SELECT COUNT(*) AS count_segment FROM segment_" + to_string(interest_point) + " WHERE sisa_koneksi=?");
            prep->setInt(1, sisa_koneksi);
            res = prep->executeQuery();
            while (res->next()) {
                size_segment = res->getInt("count_segment");
            }
            delete prep;
            delete con;
            delete res;
        }
        catch(sql::SQLException &e) {
            cout << "# ERR: SQLException in " << __FILE__;
            cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
            cout << "# ERR: " << e.what();
            cout << " (MySQL error code: " << e.getErrorCode();
            cout << ", SQLState: " << e.getSQLState() << " )" << endl;
        }
        return size_segment;
}

int segment_size(int interest_point) {
    int size_segment; 
        try {
            sql::Driver *driver;
            sql::Connection *con;
            sql::Statement *stat;
            sql::ResultSet *res;
            // sql::PreparedStatement *prep;

            driver = get_driver_instance();
            con = driver->connect(db_host, db_user, db_pass);
            con->setSchema(db_name);

            stat = con->createStatement();
            res = stat->executeQuery("SELECT COUNT(*) AS count_segment FROM segment_" + to_string(interest_point));
            while (res->next()) {
                size_segment = res->getInt("count_segment");
            }
            delete stat;
            delete con;
            delete res;
        }
        catch(sql::SQLException &e) {
            cout << "# ERR: SQLException in " << __FILE__;
            cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
            cout << "# ERR: " << e.what();
            cout << " (MySQL error code: " << e.getErrorCode();
            cout << ", SQLState: " << e.getSQLState() << " )" << endl;
        }
        return size_segment;
}