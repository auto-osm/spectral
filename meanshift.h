#ifndef _MEANSHIFT_H_
#define _MEANSHIFT_H_

#include <vector>
using namespace std;

#include "vecops.h"

double meanshiftKernelFunction(vector<double > &v1, vector<double > &v2, double sigma); 

vector<int> meanshift(vector<vector<double > > &data, double sigma);

vector<int> meanshiftFast(vector<vector<double > > &data, int imgHeight, int imgWidth, double sigma);

#endif



