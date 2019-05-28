#include "lib/segment.h"
#include <iostream>
#include "lib/data.h"

using namespace std;

int main() {
    int interest_point = 10;
    vector<segment> list_segment = segment_get_all(interest_point);
    for (int i=0;i<list_segment.size();i++) {
        if ((list_segment[i].p1.x==0 && list_segment[i].p2.x==0) || (list_segment[i].p1.y==0 && list_segment[i].p2.y==0) || (list_segment[i].p1.x==bound && list_segment[i].p2.x==bound) || (list_segment[i].p1.y==bound && list_segment[i].p2.y==bound)) {
            segment_update_sisa_koneksi(interest_point, list_segment[i].id, 1);
            segment_update_is_connected(interest_point, list_segment[i].id, 0);
        }
        else {
            segment_update_sisa_koneksi(interest_point, list_segment[i].id, 2);
            segment_update_is_connected(interest_point, list_segment[i].id, 0);
        }
        // cout << list_segment[i].id << endl;
    }
}