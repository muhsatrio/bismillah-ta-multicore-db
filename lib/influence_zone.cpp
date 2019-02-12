#include "influence_zone.h"

vector<vector<segment>> inf_compute(int counts, point query_point, vector<point> interest_points, double bound) {
    // cout << "influence_zone - inf_compute" << endl;
    int rank, size;
    MPI_Datatype struct_line = generateStructLine();
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    vector<line> bisector = pb_generate(query_point, interest_points);
    vector<vector<point>> intersects = int_generate(bisector, bound);
    vector<segment> segments = seg_generate(intersects);
    
    segment query_segment;
    unsigned int count;

    vector<vector<segment>> zone(interest_points.size());
    for (int i=0;i<segments.size();i++) {
        query_segment = segment {
            query_point,
            seg_mid_point(segments[i]),
            segments[i].from
        };
        count = inf_count_intersection(query_segment, bisector);
        zone[count].push_back(segments[i]);
    }
    // cout << "hehe" << rank << endl;
    //<-- parallelnya disini
    // if (rank==0)
    return zone;
}

void inf_compute_partial(point query_point, vector<point> interest_points, double bound) {
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // cout << "influence_zone - inf_compute_partial" << endl;
    // if (rank==0) {
    //     cout << interest_points.size() << endl;
    //     cout << "===========\n";
    //     for (int i=0;i<interest_points.size();i++) {
    //         cout << interest_points[i].x << ' ' << interest_points[i].y << endl;
    //     }
    //     cout << "===========\n";
    // }
    vector<line> bisector;
    bisector.clear();
    bisector = pb_generate(query_point, interest_points);
    // if (rank==0) {
    //     cout << interest_points.size() << endl;
    //     cout << "===========\n";
    //     for (int i=0;i<bisector.size();i++) {
    //         cout << bisector[i].c << ' ' << bisector[i].m << endl;
    //     }
    //     cout << "===========\n";
    // }
    // cout << bisector.size() << ' ' << rank << endl;
    vector<point> intersects;
    vector<segment> segments;
    segment query_segment;
    vector<tuple<segment, unsigned int>> labeled_segment;

    for (unsigned int index=0;index<bisector.size();index++) {
        intersects.clear();
        segments.clear();
        labeled_segment.clear();
        vector<point> temp = int_generate_partial(bisector, bound, index);
        // cout << "===========\n";
        // for (int i=0;i<temp.size();i++) {
        //     cout << temp[i].x << ' ' << temp[i].y << endl;
        // }
        // cout << "===========\n";
        // cout << temp.size() << ' ' << rank << endl;
        intersects.insert(intersects.end(), temp.begin(), temp.end());
        vector<segment> tempSegment = seg_compute(intersects, index);
        // cout << "===========\n";
        // for (int i=0;i<tempSegment.size();i++) {
        //     cout << tempSegment[i].start.x << ' ' << tempSegment[i].start.y << endl;
        // }
        // cout << "===========\n";
        // cout << "hehe" << endl;
        segments.insert(segments.end(), tempSegment.begin(), tempSegment.end());

        for (int i=0;i<segments.size();i++) {
            query_segment = segment {
                query_point,
                seg_mid_point(segments[i]),
                index,
            };
            unsigned int tempLabel = inf_count_intersection(query_segment, bisector);
            labeled_segment.push_back(make_tuple(segments[i], tempLabel));
        }
    out_file_zone_csv(interest_points.size(), labeled_segment);
    }
}

unsigned int inf_count_intersection(segment seg, vector<line> bisector) {
    // cout << "influence_zone - count_intersection" << endl;
    // int rank, size;
    // MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // MPI_Comm_size(MPI_COMM_WORLD, &size);
    // int count;
    // if (rank==0) {
    //     count = 0;
    //     int countTemp;
    //     for (int i=1;i<size;i++) {
    //         MPI_Recv(&countTemp, 1, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    //         count+=countTemp;
    //     }
    // }
    // else {
    //     count = 0;
    //     int len = bisector.size();
    //     for (int i=0;i<len;i+=(size-1)) {
    //         if (i+(rank-1)!=seg.from && int_line_segment(bisector[i+(rank-1)], seg)) {
    //             count++;
    //         }
    //     }
    //     MPI_Send(&count, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    // }

    // if (rank==0) {
    //     for (int i=1;i<size;i++) {
    //         MPI_Send(&count, 1, MPI_INT, i, 2, MPI_COMM_WORLD);
    //     }
    // }
    // else {
    //     MPI_Recv(&count, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    // }

    unsigned int count = 0;
    int index = 0, len = bisector.size();
    for (int i=0;i<bisector.size();i++) {
        if (i!=seg.from && int_line_segment(bisector[i], seg)) {
            count++;
        }
    }
    return count;
}

bool inf_query(point query_point, vector<vector<segment>> zone, point object, unsigned int k) {
    // cout << "influence_zone - inf_query" << endl;
    segment query_segment = segment {
        query_point,
        object,
        0
    };
    bool intersect = false;
    int index = 0;

    while (!intersect && index < zone[k].size()) {
        intersect = int_segment(query_segment, zone[k][index]);
        index++;
    }
    return !intersect;

}