#ifndef _LAPLACIAN_H_
#define _LAPLACIAN_H_

#include <math.h>
#include <stdio.h>
#include <vector>
using namespace std;

#include "cscmatrix.h"


vector<double> getCSCMatrixDiagonal(CSCMat mat);


CSCMat getCSCLaplacianMatrix(CSCMat mat);


CSCMat getCSCNormalizedLaplacianMatrix(CSCMat mat);

#endif
