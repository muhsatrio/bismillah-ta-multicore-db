#include "perpendicular_bisector.h"

point pb_calculate_mid(point point_1, point point_2) {
    // cout << "perpendicular_bisector - calculate_mid" << endl;
    return point{
        (point_1.x + point_2.x) / 2.0,
        (point_1.y + point_2.y) / 2.0
    };
};

double pb_calculate_c(double slope, point mid_point) {
    // cout << "perpendicular_bisector - calculate_c" << endl;
    return mid_point.y - mid_point.x * slope; 
};

line pb_compute(point point_1, point point_2) {
    // cout << "perpendicular_bisector - p_compute" << endl;
    double m = - 1.0 / pb_calculate_slope(point_1, point_2);
    double c = pb_calculate_c(m, pb_calculate_mid(point_1, point_2));
    return line{c, m};
};

vector<line> pb_generate(point query_point, vector<point> interest_point) {
    int rank, size;
    MPI_Datatype struct_line = generateStructLine();
    // len = interest_point.size();
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    vector<line> receive;
    if (rank==0) {
        for (int i=1;i<size;i++) {
            vector<line> tempReceive;
            int lenReceive;
            MPI_Recv(&lenReceive, 1, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            cout << lenReceive << ' ' << rank << endl;
            tempReceive.clear();
            tempReceive.resize(lenReceive);
            MPI_Recv(&tempReceive[0], lenReceive, struct_line, i, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            receive.insert(receive.end(), tempReceive.begin(), tempReceive.end());
        }
        // for (int i=0;i<receive.size();i++) {
        //     cout << receive[i].c << ' ' << receive[i].m << endl;
        // }
        // cout << "total data: " << receive.size() << endl;
        // return receive;
    }
    else {
        int len;
        len = interest_point.size();
        vector<line> temp;
        line tempLine;
        // cout << "_____________\n";
        for (int i=0;i<len;i+=(size-1)) {
            temp.push_back(pb_compute(query_point, interest_point[i+(rank-1)]));
        }
        int lenTemp = temp.size();
        cout << "lenTemp" << lenTemp << ' ' << rank << endl;
        // len = temp.size();
        MPI_Send(&lenTemp, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        MPI_Send(&temp[0], lenTemp, struct_line, 0, 2, MPI_COMM_WORLD);
        temp.clear();
    }

    if (rank==0) {
        for (int i=1;i<size;i++) {
            int bisectorLen = receive.size();
            MPI_Send(&bisectorLen, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&receive[0], bisectorLen, struct_line, i, 1, MPI_COMM_WORLD);
        }
    }
    else {
        int bisectorLen;
        MPI_Recv(&bisectorLen, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        receive.clear();
        receive.resize(bisectorLen);
        MPI_Recv(&receive[0], bisectorLen, struct_line, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    return receive;

};

double pb_calculate_slope(point point_1, point point_2) {
    // cout << "perpendicular_bisector - calculate_slope" << endl;
    return (point_2.y - point_1.y) / (point_2.x - point_1.x);
};