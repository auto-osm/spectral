#ifndef _KMEANS_H_
#define _KMEANS_H_

#include <vector>
#include <stdlib.h>
#include <time.h>

using namespace std;

#include "vecops.h"

vector<int> k_means(vector<vector<double > > dataPoints, int k);

#endif
