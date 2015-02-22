#ifndef _AFFINITY_H_
#define _AFFINITY_H_


#define GREEN  "\033[22;31m"
#define RESETCOLOR "\033[0m"

#include <stdio.h>
#include <opencv2/core/core.hpp>
using namespace cv;

#include "cscmatrix.h"
#include "math.h"


double affinityFunction(double val1, double val2, double sigma);

CSCMat getCSCAffinityMatrix(Mat mat, double sigma);

double** get2DDoubleAffinityMatrix(Mat mat, double sigma);

void print2DDoubleArray(double** arr, int rows, int cols);
void save2DDoubleArray(char* filename, double** arr, int rows, int cols);

#endif
