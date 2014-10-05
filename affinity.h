#ifndef _AFFINITY_H_
#define _AFFINITY_H_


#define GREEN  "\033[22;31m"
#define RESETCOLOR "\033[0m"

#include <stdio.h>
#include <opencv2/core/core.hpp>
using namespace cv;

#include "cscmatrix.h"
#include "math.h"


double affinityFunction(double val1, double val2);

CSCMat getCSCAffinityMatrix(Mat mat);

double** get2DdoubleAffinityMatrix(Mat mat);

void print2DdoubleArray(double** arr, int rows, int cols);

#endif
