#include "vertex.h"

void vertex_init(int point) {
        try {
            sql::Driver *driver;
            sql::Connection *con;
            sql::Statement *stat;
            // sql::ResultSet *result;

            driver = get_driver_instance();
            con = driver->connect(db_host, db_user, db_pass);
            con->setSchema(db_name);

            stat = con->createStatement();
            stat->execute("CREATE table vertex_" + to_string(point) + "(id INT NOT NULL AUTO_INCREMENT, x DOUBLE, y DOUBLE, PRIMARY KEY(id))");
            stat->execute("CREATE INDEX idx_vertex_" + to_string(point) + " ON vertex_" + to_string(point) + "(x, y)");
            stat->execute("CREATE table relation_vertex_perpendicular_" + to_string(point) + "(id INT NOT NULL AUTO_INCREMENT, id_vertex INT, vertex_x DOUBLE, vertex_y DOUBLE, id_perpendicular INT, PRIMARY KEY(id))");
            stat->execute("CREATE INDEX idx_relation_vertex_perpendicular_" + to_string(point) + " ON relation_vertex_perpendicular_" + to_string(point) + "(id_vertex, vertex_x, vertex_y, id_perpendicular)");
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

double vertex_calculate_x(line line_1, line line_2) {
    // cout << "intersection - calculate_x" << endl;
    return (line_2.c - line_1.c) / (line_1.m - line_2.m);
}

point vertex_compute(line perpendicular_1, line perpendicular_2) {
    double x = vertex_calculate_x(perpendicular_1, perpendicular_2);
    double y = (perpendicular_1.m * x) + perpendicular_1.c;
    return point{
        x, y
    };
}

void vertex_generate(int interest_point, int perpendicular_size) {
        int vertex_id;
        for (int i=1;i<perpendicular_size;i++) {
            for (int j=i+1;j<=perpendicular_size;j++) {
                // perpendicular_bisector_get(interest_point, i);
                line perpendicular_1 = perpendicular_bisector_get(interest_point, i);
                line perpendicular_2 = perpendicular_bisector_get(interest_point, j);
                vector<point> vertex_bound = vertex_compute_bound(perpendicular_1);
                point vertex_gen = vertex_compute(perpendicular_1, perpendicular_2);
                if ((vertex_gen.x>=0 && vertex_gen.x<=bound) && (vertex_gen.y>=0 && vertex_gen.y<=bound)) {
                    if (vertex_search(interest_point, vertex_gen)==0) {
                        vertex_insert(interest_point, vertex_gen);
                        // count++;
                        // vertex_id = count - 1;
                    }
                    // else {
                    vertex_id = vertex_get_id(interest_point, vertex_gen);
                    // }
                    if (vertex_relation_search(interest_point, vertex_id, i)==0) {
                        vertex_relation_insert(interest_point, vertex_id, vertex_gen.x, vertex_gen.y, i);
                    }
                    if (vertex_relation_search(interest_point, vertex_id, j)==0) {
                        vertex_relation_insert(interest_point, vertex_id, vertex_gen.x, vertex_gen.y, j);
                    }
                }
                for (int k=0;k<4;k++) {
                    if ((vertex_bound[k].x>=0 && vertex_bound[k].x<=bound) && (vertex_bound[k].y>=0 && vertex_bound[k].y<=bound)) {
                        if (vertex_search(interest_point, vertex_bound[k])==0) {
                            vertex_insert(interest_point, vertex_bound[k]);
                            // count++;
                            // vertex_id = count - 1;
                        }
                        // else {
                        vertex_id = vertex_get_id(interest_point, vertex_bound[k]);
                        // }
                        if (vertex_relation_search(interest_point, vertex_id, i)==0) {
                            vertex_relation_insert(interest_point, vertex_id, vertex_bound[k].x, vertex_bound[k].y, i);
                        }
                        if (vertex_relation_search(interest_point, vertex_id, -k)==0) {
                            vertex_relation_insert(interest_point, vertex_id, vertex_bound[k].x, vertex_bound[k].y, -k);
                        }
                    }
                }
            }
        }
        // get vertex bound for perpendicular last index
        line perpendicular_last = perpendicular_bisector_get(interest_point, perpendicular_size);
        vector<point> vertex_bound = vertex_compute_bound(perpendicular_last);
        vertex_bound.push_back(point{0, 0});
        vertex_bound.push_back(point{0, bound});
        vertex_bound.push_back(point{bound, 0});
        vertex_bound.push_back(point{bound, bound});
        for (int k=0;k<8;k++) {
            if ((vertex_bound[k].x>=0 && vertex_bound[k].x<=bound) && (vertex_bound[k].y>=0 && vertex_bound[k].y<=bound)) {
                if (vertex_search(interest_point, vertex_bound[k])==0) {
                    vertex_insert(interest_point, vertex_bound[k]);
                    // count++;
                    // vertex_id = count - 1;
                }
                // else {
                vertex_id = vertex_get_id(interest_point, vertex_bound[k]);
                // }
                if (k<4) {
                    if (vertex_relation_search(interest_point, vertex_id, perpendicular_size)==0) {
                        vertex_relation_insert(interest_point, vertex_id, vertex_bound[k].x, vertex_bound[k].y, perpendicular_size);
                    }
                    if (vertex_relation_search(interest_point, vertex_id, -k)==0) {
                        vertex_relation_insert(interest_point, vertex_id, vertex_bound[k].x, vertex_bound[k].y, -k);
                    }
                }
                else {
                    if (vertex_bound[k].x==0) {
                        if (vertex_relation_search(interest_point, vertex_id, 0)==0) {
                            vertex_relation_insert(interest_point, vertex_id, vertex_bound[k].x, vertex_bound[k].y, 0);
                        }
                    }
                    else if (vertex_bound[k].x==bound) {
                        if (vertex_relation_search(interest_point, vertex_id, -1)==0) {
                            vertex_relation_insert(interest_point, vertex_id, vertex_bound[k].x, vertex_bound[k].y, -1);
                        }
                    }
                    if (vertex_bound[k].y==0) {
                        if (vertex_relation_search(interest_point, vertex_id, -2)==0) {
                            vertex_relation_insert(interest_point, vertex_id, vertex_bound[k].x, vertex_bound[k].y, -2);
                        }
                    }
                    else if (vertex_bound[k].y==bound) {
                        if (vertex_relation_search(interest_point, vertex_id, -3)==0) {
                            vertex_relation_insert(interest_point, vertex_id, vertex_bound[k].x, vertex_bound[k].y, -3);
                        }
                    }
                }
            }
        }
}

vector<point> vertex_compute_bound(line Line) {
    vector<point> Points;

    // Vertex in x axis = 0
    Points.push_back(point{0, Line.c});
    // Vertex in x axis = bound
    Points.push_back(point{bound, (Line.m * bound) + Line.c});
    // Vertex in y axis = 0
    Points.push_back(point{-Line.c / Line.m, 0});
    // Vertex in y axis = bound
    Points.push_back(point{(bound - Line.c) / Line.m, bound});
    return Points;
}

void vertex_insert(int interest_points, point vertex_gen) {
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
            prep = con->prepareStatement("INSERT INTO vertex_" + to_string(interest_points) + "(x, y) VALUES(?, ?)");
            prep->setDouble(1, vertex_gen.x);
            prep->setDouble(2, vertex_gen.y);
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

point vertex_get(int interest_point, int id) {
        point vertex_result;
        try {
            sql::Driver *driver;
            sql::Connection *con;
            sql::Statement *stat;
            sql::ResultSet *res;

            driver = get_driver_instance();
            con = driver->connect(db_host, db_user, db_pass);
            con->setSchema(db_name);

            stat = con->createStatement();
            res = stat->executeQuery("SELECT * FROM vertex_" + to_string(interest_point) + " WHERE id=" + to_string(id));
            while (res->next()) {
                vertex_result.x = res->getDouble("x");
                vertex_result.y = res->getDouble("y");
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
        return vertex_result;
}



int vertex_search(int interest_point, point vertex_obj) {
        // fixed << setprecision(15) << vertex_obj.x;
        // fixed << setprecision(15) << vertex_obj.y;
        // cout << vertex_obj.x << ' ' << vertex_obj.y << endl;
        int count_result = 0;
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
            prep = con->prepareStatement("SELECT * FROM vertex_" + to_string(interest_point) + " WHERE x=? AND y=?");
            prep->setDouble(1, vertex_obj.x);
            prep->setDouble(2, vertex_obj.y);
            res = prep->executeQuery();
            count_result = res->rowsCount();
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
        return count_result;
}

int vertex_relation_search(int interest_point, int id_vertex, int id_perpendicular) {
        int count_result = 0;
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
            prep = con->prepareStatement("SELECT * FROM relation_vertex_perpendicular_" + to_string(interest_point) + " WHERE id_vertex=? AND id_perpendicular=?");
            prep->setInt(1, id_vertex);
            prep->setInt(2, id_perpendicular);
            res = prep->executeQuery();
            count_result = res->rowsCount();
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
        return count_result;
}

void vertex_relation_insert(int interest_point, int id_vertex, double vertex_x, double vertex_y, int id_perpendicular) {
        int count_result = 0;
        try {
            sql::Driver *driver;
            sql::Connection *con;
            sql::Statement *stat;
            // sql::ResultSet *res;
            sql::PreparedStatement *prep;

            driver = get_driver_instance();
            con = driver->connect(db_host, db_user, db_pass);
            con->setSchema(db_name);

            stat = con->createStatement();
            prep = con->prepareStatement("INSERT INTO relation_vertex_perpendicular_" + to_string(interest_point) + "(id_vertex, vertex_x, vertex_y, id_perpendicular) VALUES(?, ?, ?, ?)");
            prep->setInt(1, id_vertex);
            prep->setDouble(2, vertex_x);
            prep->setDouble(3, vertex_y);
            prep->setInt(4, id_perpendicular);
            prep->execute();
            delete prep; 
            delete con;
            delete stat;
            // delete res;
        }
        catch(sql::SQLException &e) {
            cout << "# ERR: SQLException in " << __FILE__;
            cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
            cout << "# ERR: " << e.what();
            cout << " (MySQL error code: " << e.getErrorCode();
            cout << ", SQLState: " << e.getSQLState() << " )" << endl;
        }
}

int vertex_get_id(int interest_point, point vertex_search) {
        int vertex_id;
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
            prep = con->prepareStatement("SELECT * FROM vertex_" + to_string(interest_point) + " WHERE x=? AND y=?");
            prep->setDouble(1, vertex_search.x);
            prep->setDouble(2, vertex_search.y);
            res = prep->executeQuery();
            while (res->next()) {
                vertex_id = res->getInt("id");
                // vertex_result.x = res->getDouble("x");
                // vertex_result.y = res->getDouble("y");
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
        return vertex_id;
}

