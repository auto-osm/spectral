#ifndef _MEANSHIFT_H_
#define _MEANSHIFT_H_

#include <vector>
using namespace std;

#include "vecops.h"

vector<int> cluster(vector<vector<double > > &means, double threshold);

vector<vector< int > > cluster2(vector<vector<double > > &means, double threshold);

double meanshiftKernelFunction(vector<double > &v1, vector<double > &v2, double sigma); 

vector<vector<double > > meanshift(vector<vector<double > > &data, double sigma);

vector<vector<double > > meanshiftFast(vector<vector<double > > &data, int imgHeight, int imgWidth, double sigma);

#endif



