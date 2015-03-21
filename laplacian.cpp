#include "laplacian.h"
vector<double> getCSCMatrixDiagonal(CSCMat mat) {
    vector<double> diagonal(mat.pCol.size()-1,0.0f);
    /*sum values in rows*/
    for(size_t i = 0; i < mat.iRow.size();i++) {
		if(mat.val[i] != -1.0f) {
			diagonal[mat.iRow[i]]+=mat.val[i];
		}
	}

    /*sum values in columns*/
	for(size_t i = 0; i < mat.pCol.size()-1;i++) {
		for(int j = mat.pCol[i]; j < mat.pCol[i+1]; j++) {
			if(mat.val[j] != -1.0f) {
				diagonal[i]+=mat.val[j];
			}
		}
	}    
    return diagonal;
}

CSCMat getCSCLaplacianMatrix(CSCMat mat) {
    /*
        Simple Laplacian Matrix Ls = D - A
    */
    CSCMat laplacian = mat;
    vector<double> diagonal =  getCSCMatrixDiagonal(laplacian);
    int counter = 0;
    for(size_t i = 0; i < laplacian.val.size(); i++) {
        if(laplacian.val[i] == -1.0) {
            laplacian.val[i] = diagonal[counter];
            counter++;
        } else {
            laplacian.val[i] *= -1.0;
        }
    }
    return laplacian;
}

CSCMat getCSCLaplacianSym(CSCMat mat) {
    /*
        Normalized symetrical laplacian matrix L sym  = I - D^-1/2 * A * D^-1/2
    */
    printf("Creating normalized symetric laplacian matrix L sym = I - D^-1/2 * A * D^-1/2...");
    CSCMat laplacian = mat;
    vector<double> diagonal =  getCSCMatrixDiagonal(laplacian);

    int curCol = 0;
    int nextColStartInd = laplacian.pCol[curCol+1];

    for(size_t i = 0; i < laplacian.val.size(); i++) {
        if(nextColStartInd == i) {
            curCol++;
            nextColStartInd = laplacian.pCol[curCol+1];
        }
        if(laplacian.val[i] == -1.0) {
            laplacian.val[i] = 1.0;
        } else {
            double val = -1.0* laplacian.val[i] / (sqrt(diagonal[laplacian.iRow[i]]*diagonal[curCol])); 
            //printf("i %d val %f (d1 %f d2 %f) = res %f\n",i,laplacian.val[i],sqrt(diagonal[laplacian.iRow[i]]),sqrt(diagonal[curCol]),val);            
            laplacian.val[i] *=  (-1.0 / (sqrt(diagonal[laplacian.iRow[i]])*sqrt(diagonal[curCol])));   
        }
    }
    printf("[OK]\n");
    return laplacian;
}

CSCMat getCSCLaplacianRW(CSCMat aff_mat) {
    /*
        IN PROGRESS NOT SYMETRICAL
        Normalized symetrical laplacian matrix L rw = I - D^-1/2 * A 
    */
    printf("Creating normalized symetric laplacian matrix L RW = D^-1 * L");
    CSCMat laplacian = aff_mat;
    vector<double> diagonal =  getCSCMatrixDiagonal(laplacian);

    int last_ind = -1;
    int row_ind = 0;
    for(size_t i = 0; i < laplacian.val.size(); i++) {
        /*if(laplacian.rowInd[i] < last_ind) {
            row_ptr++;
        }
        last_ind = laplacian.pCol[i];
        if(laplacian.val[i] == -1.0) {
            laplacian.val[i] = 1.0;
        } else {
            printf("%d, %f / %f\n",row_ptr, laplacian.val[i],diagonal[row_ptr]);           
            laplacian.val[i] = laplacian.val[i] / diagonal[row_ptr];
        }*/

        if(laplacian.val[i] == -1.0) {
            laplacian.val[i] = 1.0;
        } else {
            laplacian.val[i] = -1.0 * laplacian.val[i] / diagonal[laplacian.iRow[i]];
        } 
    }
    return laplacian;
}
