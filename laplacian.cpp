#include "laplacian.h"


vector<double> getCSCMatrixDiagonal(CSCMat mat)
{
   
    
    vector<double> diagonal(mat.pCol.size()-1,0.0f);

    /*sum values in rows*/
    for(size_t i = 0; i < mat.iRow.size();i++)
	{
		if(mat.val[i] != -1.0f)
		{
			diagonal[mat.iRow[i]]+=mat.val[i];
		}
	}

    /*sum values in columns*/
	for(size_t i = 0; i < mat.pCol.size()-1;i++)
	{
		for(int j = mat.pCol[i]; j < mat.pCol[i+1]; j++)
		{
			if(mat.val[j] != -1.0f)
			{
				diagonal[i]+=mat.val[j];
			}
		}
	}

    
    return diagonal;

}

CSCMat getCSCLaplacianMatrix(CSCMat mat)
{
    /*
        Simple Laplacian Matrix Ls = D - A
    */
    CSCMat laplacian = mat;
    vector<double> diagonal =  getCSCMatrixDiagonal(laplacian);
    int counter = 0;
    for(size_t i = 0; i < laplacian.val.size(); i++)
    {
        if(laplacian.val[i] == -1.0)
        {
            laplacian.val[i] = diagonal[counter];
            counter++;
        }
        else
        {
            laplacian.val[i] *= -1.0;
        }

    }
    return laplacian;

}

CSCMat getCSCNormalizedLaplacianMatrix(CSCMat mat)
{
    /*
        Normalized symetrical laplacian matrix L  = I - D^-1/2 * A * D^-1/2
    */

    printf("Creating normalized symetric laplacian matrix L = I - D^-1/2 * A * D^-1/2...");
    CSCMat laplacian = mat;
    vector<double> diagonal =  getCSCMatrixDiagonal(laplacian);


    int curCol = 0;
    int nextColStartInd = laplacian.pCol[curCol+1];

    for(size_t i = 0; i < laplacian.val.size(); i++)
    {
        if(nextColStartInd == i)
        {
            curCol++;
            nextColStartInd = laplacian.pCol[curCol+1];
        }

        if(laplacian.val[i] == -1.0)
        {
            laplacian.val[i] = 1.0;
        }
        else
        {


            double val = - -1.0* laplacian.val[i] / (sqrt(diagonal[laplacian.iRow[i]]*diagonal[curCol])); 
            //printf("i %d val %f (d1 %f d2 %f) = res %f\n",i,laplacian.val[i],sqrt(diagonal[laplacian.iRow[i]]),sqrt(diagonal[curCol]),val);

            
            laplacian.val[i] *=  (-1.0 / (sqrt(diagonal[laplacian.iRow[i]])*sqrt(diagonal[curCol])));

            
        }

    }
    printf("[OK]\n");
    return laplacian;

}
