#include "intersection.h"

bool int_rules_sorting(point a, point b) {
    // cout << "intersection - rules_sorting" << endl;
    if (a.x!=b.x) {
        return a.x < b.x;
    }
    if (a.y!=b.y) {
        return a.y < b.y;
    }
}

vector<point> int_sorting(vector<point> points) {
    // cout << "intersection - sorting" << endl;
    vector<point> sorted_;
    sorted_ = points;
    sort(sorted_.begin(), sorted_.end(), int_rules_sorting);
    return sorted_;
}

vector<vector<point>> int_generate(vector<line> lines, double bound) {
    // cout << "intersection - i_generate" << endl;
    int rank,size;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Datatype struct_point = generateStructPoint();
    unsigned int outer = 0;
    unsigned int inner;
    int len = lines.size();
    vector<vector<point>> points;
    // bismillah harus kelar malem ini anjeng!!

    if (rank==0) {
        vector<point> tempRecv;
        int lenRecv;
        int idx = 0;

        while (idx<len) {
            for (int i=1;i<size;i++) {
                MPI_Recv(&lenRecv, 1, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                tempRecv.clear();
                tempRecv.resize(lenRecv);
                MPI_Recv(&tempRecv[0], lenRecv, struct_point, i, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                points.push_back(tempRecv);
            }
            idx+=(size-1);
        }
    }
    else {
        int lenSend;
        vector<point> point_;
        vector<point> cleaned;
        while (outer < len) {
            inner = 0;
            point_.clear();
            cleaned.clear();
            while (inner < lines.size()) {
                if (inner != outer) {
                    point_.push_back(int_compute(lines[outer+(rank-1)], lines[inner]));
                }
                inner++;
            }
            vector<point> temp;
            temp = int_compute_bound(lines[outer+(rank-1)],bound);
            point_.insert(point_.end(), temp.begin(), temp.end());
            temp = int_sorting(int_remove_outside_bound(point_, bound));
            cleaned.insert(cleaned.end(), temp.begin(), temp.end());
            lenSend = cleaned.size();
            MPI_Send(&lenSend, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
            MPI_Send(&cleaned[0], lenSend, struct_point, 0, 2, MPI_COMM_WORLD);
            outer+=(size-1);
        }    
    }
    if (rank==0) {
        int pointSize,zoneSize;
        zoneSize = points.size();
        for (int i=1;i<size;i++) {
            MPI_Send(&zoneSize, 1, MPI_INT, i, 3, MPI_COMM_WORLD);
        }
        for (int i=1;i<size;i++){
            for (int j=0;j<points.size();j++) {
                pointSize = points[j].size();
                MPI_Send(&pointSize, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
                MPI_Send(&points[j][0], pointSize, struct_point, i, 2, MPI_COMM_WORLD);
            }
        }
    }
    else {
        vector<point> tempPoint;
        int pointSize,zoneSize;
        MPI_Recv(&zoneSize, 1, MPI_INT, 0, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (int i=0;i<zoneSize;i++) {
            MPI_Recv(&pointSize, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            tempPoint.clear();
            tempPoint.resize(pointSize);
            MPI_Recv(&tempPoint[0], pointSize, struct_point, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            points.push_back(tempPoint);
        }
    }
 
    return points;

}

vector<point> int_generate_partial(vector<line> lines, double bound, unsigned int index) {
    // cout << "intersection - generate_partial" << endl;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Datatype struct_point = generateStructPoint();
    vector<point> intersections;
    vector<point> temp;
    vector<point> result;
    // int inner = 0;
    // for (int i=0;i<lines.size();i++) {
    //     if (i!=index) {
    //         intersections.push_back(int_compute(lines[index], lines[i]));    
    //     }
    // }

    if (rank==0) {
        vector<point> temp;
        int len;
        for (int i=1;i<size;i++) {
            MPI_Recv(&len, 1, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            temp.clear();
            temp.resize(len);
            intersections.insert(intersections.end(), temp.begin(), temp.end());
        }
    }
    else {
        int len = lines.size();
        for (int i=0;i<len;i+=(size-1)) {
            if ((i+(rank-1))!=index) {
                intersections.push_back(int_compute(lines[index], lines[i+(rank-1)]));    
            }
        }
        len = intersections.size();
        
        MPI_Send(&len, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        MPI_Send(&intersections[0], len, struct_point, 0, 2, MPI_COMM_WORLD);
        
    }
    if (rank==0) {
        int len = intersections.size();
        for (int i=1;i<size;i++) {
            MPI_Send(&len, 1, MPI_INT, i, 3, MPI_COMM_WORLD);
            MPI_Send(&intersections[0], len, struct_point, i, 4, MPI_COMM_WORLD);
        }
    }
    else {
        int len;
        MPI_Recv(&len, 1, MPI_INT, 0, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        intersections.clear();
        intersections.resize(len);
        MPI_Recv(&intersections[0], len, struct_point, 0, 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // cout << intersections.size() << ' ' << rank << endl;

    temp = int_compute_bound(lines[index], bound);
    intersections.insert(intersections.end(), temp.begin(), temp.end());
    result = int_sorting(int_remove_outside_bound(intersections, bound));
    return result;
};

double int_calculate_x(line line_1, line line_2) {
    // cout << "intersection - calculate_x" << endl;
    return (line_2.c - line_1.c) / (line_1.m - line_2.m);
}

point int_compute(line line_1, line line_2) {
    // cout << "intersection - i_compute" << endl;
    double x = int_calculate_x(line_1, line_2);
    double y = (line_1.m * x) + line_1.c;
    return point{
        x, y
    };
}

vector<point> int_compute_bound(line line_, double bound) {
    vector<point> points;

    point point_ = point{
        0.0,
        line_.c
    };
    points.push_back(point_);

    point_ = point{
        bound,
        (line_.m * bound) + line_.c
    };
    points.push_back(point_);
    
    point_ = point{
        -line_.c / line_.m,
        0.0
    };
    points.push_back(point_);

    point_ = point{
        (bound - line_.c) / line_.m,
        bound
    };
    points.push_back(point_);
    
    return points;

}

vector<point> int_remove_outside_bound(vector<point> points, double bound) {
    vector<point> cleaned;
    for (int i=0;i<points.size();i++) {
        if (points[i].x >= 0.0 && points[i].x <= bound && points[i].y >= 0.0 && points[i].y <= bound) {
            cleaned.push_back(points[i]);
        }
    }
    return cleaned;
} 

bool int_line_segment(line line_, segment segment_) {
    double t = (line_.c - segment_.start.y + line_.m * segment_.start.x) / (segment_.end.y - segment_.start.y + line_.m * (segment_.start.x - segment_.end.x));
    return t>0.0 && t<1.0;
}
unsigned int int_calculate_orientation(point point_1, point point_2, point point_3) {
    double val = (point_2.y - point_1.y) * (point_3.x - point_2.x) - (point_2.x - point_1.x) * (point_3.y - point_2.y);
    if (val==0)
        return 0;
    else if (val>0)
        return 1;
    else
        return 2;
}


bool int_segment(segment segment_1, segment segment_2) {
    unsigned int orientation_1 = int_calculate_orientation(segment_1.start, segment_1.end, segment_2.start);
    unsigned int orientation_2 = int_calculate_orientation(segment_1.start, segment_1.end, segment_2.end);
    unsigned int orientation_3 = int_calculate_orientation(segment_2.start, segment_2.end, segment_1.start);
    unsigned int orientation_4 = int_calculate_orientation(segment_2.start, segment_2.end, segment_1.end);
    return orientation_1 != orientation_2 && orientation_3 != orientation_4;
}