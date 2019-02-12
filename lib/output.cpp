#include "output.h"

void out_create_file_time() {
    fstream filestr;
    filestr.open("time.csv", fstream::in | fstream::out | fstream::app);
    filestr << "interest points,seconds\n";
}

void out_save_time(unsigned int count, double duration) {
    // cout << "output - save_time" << endl;
    string path = "time.csv";
    fstream create_file;
    create_file.open(path, fstream::in | fstream::out | fstream::app);
    create_file << count << "," << duration << "\n";
}

void out_create_file_segment() {
    fstream filestr;
    filestr.open("segment.csv", fstream::in | fstream::out | fstream::app);
    filestr << "interest points,segment\n";
}

void out_save_segment_file(unsigned int interests, unsigned int average) {
    // cout << "output - save_segment_file" << endl;
    string path = "segment.csv";
    fstream create_file;
    create_file.open(path, fstream::in | fstream::out | fstream::app);
    create_file << interests <<"," << average <<  "\n";
    // create_file.close();
}

void out_save_query_time(unsigned int k, unsigned int objects, time_t duration) {
    // cout << "output - save_query_time" << endl;
    string path = "query.csv";
    ofstream create_file;
    create_file.open(path);
    create_file << "k,objects,second\n";
    create_file << k <<"," << objects << "," << duration << "\n";
    // create_file.close();
}

void out_create_file_size() {
    fstream filestr;
    filestr.open("size.csv", fstream::in | fstream::out | fstream::app);
    filestr << "interest points,segment\n";
}

void out_save_file_size(unsigned int k, unsigned int avg_segment) {
    // cout << "output - save_file_size" << endl;
    double size = (double)k * (double)avg_segment * 0.000103;
    string path = "size.csv";
    fstream create_file;
    create_file.open(path, fstream::in | fstream::out | fstream::app);
    create_file << k <<"," << size << "\n";
    // create_file.close();
}

void out_save_query_csv(unsigned int interest_points, point query_point) {
    // cout << "output - save_query_csv" << endl;
    string path = to_string(interest_points) + "_query_point.csv";
    ofstream create_file;
    create_file.open(path);
    create_file << "x,y\n";
    create_file << query_point.x << "," << query_point.y << "\n";
    // create_file.close();
}

void out_create_interest_csv(vector<point> interest_points) {
    // cout << "output - create_interest_csv" << endl;
    int len = interest_points.size();
    string path = to_string(len) + "_interest_points.csv";
    ofstream create_file;
    create_file.open(path);
    create_file << "x,y\n";
    for (int i=0;i<interest_points.size();i++) {
        create_file << interest_points[i].x << "," << interest_points[i].y << "\n"; 
    }
    create_file.close();
}

void out_file_zone_csv(unsigned int interest_points, vector<tuple<segment, unsigned int>> labeled_segment) {
    // cout << "output - file_zone_csv" << endl;
    string path = to_string(interest_points) + "_zone.csv";
    ofstream myfile;
    myfile.open(path);
    myfile << "x1,x2,y1,y2,k\n";
    for (tuple<segment, unsigned int> t: labeled_segment) {
        myfile << get<0>(t).start.x << "," << get<0>(t).start.y << "," << get<0>(t).end.x << "," << get<0>(t).end.y << "," << get<1>(t) << "\n";
    }
    // myfile.close();
}

void out_zone_csv(vector<vector<segment>> zone, unsigned int count) {
    // int len = zone.size();
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (rank==0) {
        string path = to_string(count) + "_zone.csv";
        ofstream myfile;
        myfile.open(path);
        myfile << "x1,y1,x2,y2,k\n";
        for (int i=0;i<zone.size();i++) {
            for (int j=0;j<zone[i].size();j++) {
                myfile << zone[i][j].start.x << "," << zone[i][j].start.y << "," << zone[i][j].end.x << "," << zone[i][j].end.y << "," << i << "\n"; 
            }
        }
    } 
    // for (tuple<segment, unsigned int> t: labeled_segment) {
    //     myfile << get<0>(t).start.x << "," << get<0>(t).start.y << "," << get<0>(t).end.x << "," << get<0>(t).end.y << "," << get<1>(t) << "\n";
    // }
}

void out_point_csv(vector<point> interest_points) {
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int len = interest_points.size();
    if (rank==0) {
        string path = to_string(len) + "_zone.csv";
        ofstream myfile;
        myfile.open(path);
        myfile << "x,y\n";
        for (int i=0;i<interest_points.size();i++) {
            myfile << interest_points[i].x << "," << interest_points[i].y << "\n"; 
        }
    }
}