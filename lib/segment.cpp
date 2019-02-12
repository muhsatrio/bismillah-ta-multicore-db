#include "segment.h"

// using namespace std;

vector<segment> seg_compute(vector<point> points, unsigned int from) {
    // cout << "segment - s_compute" << endl;
    int index = 0;
    vector<segment> segments;

    if (points.size()>0) {
        while (index < points.size() - 1) {
            segments.push_back(segment {
                points[index],
                points[index+1],
                from
            });
            index++;
        }
    }
    return segments;
}

vector<segment> seg_generate(vector<vector<point>> points) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Datatype struct_point = generateStructPoint();
    MPI_Datatype struct_segment = generateStructSegment(struct_point);
    // cout << "segment - generate_segment" << endl;
    vector<segment> segments;
    vector<segment> temp;
    // int i = 0;
    int lenPoints = points.size();
    if (rank==0) {
        int lenSegment;
        vector<segment> tempSegment;
        for (int i=1;i<size;i++) {
            MPI_Recv(&lenSegment, 1, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            tempSegment.clear();
            tempSegment.resize(lenSegment);
            MPI_Recv(&tempSegment[0], lenSegment, struct_segment, i, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            segments.insert(segments.end(), tempSegment.begin(), tempSegment.end());
        }
    }
    else {
        for (int i=0;i<lenPoints;i+=(size-1)) {
            temp = seg_compute(points[i+(rank-1)], i+(rank-1));
            segments.insert(segments.end(), temp.begin(), temp.end());
        }
        int lenSegments = segments.size();
        MPI_Send(&lenSegments, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        MPI_Send(&segments[0], lenSegments, struct_segment, 0, 2, MPI_COMM_WORLD);
    }
    if (rank==0) {
        for (int i=1;i<size;i++) {
            int lenSegments = segments.size();
            MPI_Send(&lenSegments, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&segments[0], lenSegments, struct_segment, i, 2, MPI_COMM_WORLD);
        }
    }
    else {
        int lenSegments;
        MPI_Recv(&lenSegments, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        segments.clear();
        segments.resize(lenSegments);
        MPI_Recv(&segments[0], lenSegments, struct_segment, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    // for (int i=0;i<lenPoints;i+=(size-1)) {
    //     temp = seg_compute(points[i+(rank-1)], i+(rank-1));
    //     segments.insert(segments.end(), temp.begin(), temp.end());
    // }
    return segments;
};

point seg_mid_point(segment sgmnt) {
    // cout << "segment - mid_point" << endl;
    return point {
        (sgmnt.start.x + sgmnt.end.x) / 2.0,
        (sgmnt.start.y + sgmnt.end.y) / 2.0,
    };
}