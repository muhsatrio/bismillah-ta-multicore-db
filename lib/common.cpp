#include "common.h"

using namespace std;

unsigned int com_average_segments(vector<vector<segment>> influence_zone) {
    // cout << "common - average_segments" << endl;
    unsigned int count = 0;
    unsigned int segment_count = 0;
    for (int i=0;i<influence_zone.size();i++) {
        if (influence_zone[i].size()>0) {
            segment_count++;
        }
        count = count + influence_zone[i].size();
    }
    cout << "hihi" << endl;
    cout << "count: " << count << endl;
    cout << "segment: " << segment_count << endl;
    return count/segment_count;
    // return count;

    // for (vector<vector<segment>>::iterator zone=influence_zone.begin();zone!=influence_zone.end();++zone) {
    //     if (zone->size()>0) {
    //         segment_count++;
    //     }
    //     count = count + zone->size();
    // } 
    // return count/segment_count;
}