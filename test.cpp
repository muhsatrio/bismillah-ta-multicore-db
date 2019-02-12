#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// vector<int> a(1000000000);

int main() {
    vector<vector<int>> a;
    vector<int> temp;
    temp.push_back(1);
    a.push_back(temp);
    cout << a[0][0] << endl;
    // a[1000000000] = 1;
}