#ifndef _VECOPS_H_
#define _VECOPS_H_

#include <math.h>
#include <vector>
#include <stdlib.h>

#include <stdio.h>
using namespace std;

#include "math.h"

void vecPrint(vector<double> &v);
void vecPrint(vector<int> &v);
void vecPrint2DArray(vector<vector<double > > &vec2D);

void vecSave(char* filename, vector<double > &v);
void vecSave(FILE* f, vector<double > &v);
void vecSave2DArray(char* filename, vector<vector<double > > &vec2D);

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

double vecDotProduct(vector<double> &v1, vector<double> &v2);
#endif
