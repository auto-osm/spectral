#ifndef _VECOPS_H_
#define _VECOPS_H_

#include <math.h>
#include <vector>
#include <stdlib.h>

#include <stdio.h>
using namespace std;

#include "math.h"


void vecPrint(vector<double> &v);


void vecSum(vector<double> &v1, vector<double> &v2);


void vecDif(vector<double> &v1, vector<double> &v2);


void vecDiv(vector<double> &v1, double alpha);


void vecScale(vector<double> &v1, double alpha);


vector<double> vecGetScale(vector<double> &v1, double alpha);


double vecGetNorm(vector<double> &v1);


void vecNormalize(vector<double> &v1);

void vecNormalize(vector<vector<double > > &v1);


double vecEuclidDist(vector<double> &v1, vector<double> &v2);


void vecRandomize(vector<double> &v1);


void print2DVecArray(vector<vector<double > > &vec2D);


double vecDotProduct(vector<double> &v1, vector<double> &v2);


#endif
