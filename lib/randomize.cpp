#include "randomize.h"

point rand_generate(double bound) {
    // cout << "randomize - generatePoint" << endl;
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    uniform_real_distribution<double> distribution(0.0, bound);
    return point {
        distribution(generator),
        distribution(generator)
    };
}