#ifndef _CSCMATRIX_H_
#define _CSCMATRIX_H_
#include <stdio.h>
#include <vector>
using namespace std;

struct CSCMat {
    /*
    Compressed sparse column (CSC or CCS) Matrix structure
    */
    vector<double> val;
    vector<int> iRow;
    vector<int> pCol;
};

void printCSCMatrix(CSCMat mat);

#endif
