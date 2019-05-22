#include <iostream>
#include "lib/data.h"
#include "lib/segment.h"
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include "mysql-cppconn-8/jdbc/mysql_connection.h"
#include "mysql-cppconn-8/jdbc/mysql_driver.h"
#include "mysql-cppconn-8/jdbc/cppconn/exception.h"
#include "mysql-cppconn-8/jdbc/cppconn/resultset.h"
#include "mysql-cppconn-8/jdbc/cppconn/statement.h"
#include "mysql-cppconn-8/jdbc/cppconn/prepared_statement.h"

using namespace std;

int convert_string_int(string s) {
    // cout <<"s: "<< s << endl;
    int len = s.length() - 1, pengali = 1, result = 0;
    while (len>=0) {
        result+=(((int)s[len] - 48) * pengali);
        pengali*=10;
        len--;
    }
    return result;
}

void create_table_region(int interest_point, int parallel_size) {
    try {
            sql::Driver *driver;
            sql::Connection *con;
            sql::Statement *stat;

            driver = get_driver_instance();
            con = driver->connect(db_host, db_user, db_pass);
            con->setSchema(db_name);

            stat = con->createStatement();
            stat->execute("CREATE table region_" + to_string(interest_point) + "_"+ to_string(parallel_size) +"(id INT NOT NULL AUTO_INCREMENT, x DOUBLE, y DOUBLE, order_val INT, label_region INT, parallel_rank INT, PRIMARY KEY(id))");
            stat->execute("CREATE INDEX region_" + to_string(interest_point) + "_"+ to_string(parallel_size) +" ON region_" + to_string(interest_point) + "_" + to_string(parallel_size) +"(id, x, y)");
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

void create_table_result() {
    try {
            sql::Driver *driver;
            sql::Connection *con;
            sql::Statement *stat;

            driver = get_driver_instance();
            con = driver->connect(db_host, db_user, db_pass);
            con->setSchema(db_name);

            stat = con->createStatement();
            stat->execute("CREATE table result_region(id INT NOT NULL AUTO_INCREMENT, total_generate_point INT, total_parallel INT, time DOUBLE DEFAULT 0, PRIMARY KEY(id))");
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

void create_table_record(int interest_point, int parallel_size) {
    try {
            sql::Driver *driver;
            sql::Connection *con;
            sql::Statement *stat;

            driver = get_driver_instance();
            con = driver->connect(db_host, db_user, db_pass);
            con->setSchema(db_name);

            stat = con->createStatement();
            stat->execute("CREATE table record_region_" + to_string(interest_point) + "_"+ to_string(parallel_size) +"(id INT NOT NULL AUTO_INCREMENT, id_segment INT, id_parallel INT, segment_size INT, time_created TIMESTAMP, PRIMARY KEY(id))");
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

int main() {
    vector<int> interest_point;
    vector<int> parallel_size;
    ifstream file_txt;
    file_txt.open("interest_point.txt");
    string temp;
    if (file_txt.is_open()) {
        while (getline(file_txt, temp)) {
            // int val = stoi(temp);
            int val = convert_string_int(temp);
            interest_point.push_back(val);
        }
    }
    file_txt.close();
    file_txt.clear();
    string temp2;
    file_txt.open("parallel.txt");
    if (file_txt.is_open()) {
        while (getline(file_txt, temp2)) {
            // int val = stoi(temp2);
            int val = convert_string_int(temp2);
            parallel_size.push_back(val);
        }
    }
    file_txt.close();
    file_txt.clear();
    // for (int i=0;i<interest_point.size();i++) {
    //     cout << interest_point[i] << endl;
    // }
    // cout << "---" << endl;
    // for (int i=0;i<parallel_size.size();i++) {
    //     cout << parallel_size[i] << endl;
    // }
    for (int i=0;i<interest_point.size();i++) {
        for (int j=0;j<parallel_size.size();j++) {
            create_table_region(interest_point[i], parallel_size[j]);
            create_table_record(interest_point[i], parallel_size[j]);         
        }
    }
    create_table_result();
}
