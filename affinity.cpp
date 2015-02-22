#include "affinity.h"

double affinityFunction(double val1, double val2, double sigma) {
    /*
        Defines affinity between two values
    */
    double diff = val1-val2;
    double affinity = gaussianFunction(diff,sigma);
    //printf("%f %f %f\n",val1,val2,affinity);
        
    return affinity;
}

CSCMat getCSCAffinityMatrix(Mat mat, double sigma) {
    /*
        Returns Lower triangle CSC affinity matrix - suitable for large matrices
        Index base 0 - search for //index base 0 
        4 neigbourhood
    */
    printf("Creating lower triangular data affinity matrix in CSC format...");
    CSCMat affinityMatrix;
    double affinity;

    for(int i = 0; i < mat.rows; i++) {
        for(int j = 0; j < mat.cols; j++) {   
                     
            affinityMatrix.pCol.push_back(affinityMatrix.val.size()); //index base 0
                                     
            affinityMatrix.val.push_back(-1.0f);
            affinityMatrix.iRow.push_back(j+i*mat.cols);
       
            if(j+1 < mat.cols) {
                affinity = affinityFunction(mat.at<double>(i,j),mat.at<double>(i,j+1), sigma);
                if(affinity != 0.0) {                                       
                    affinityMatrix.val.push_back(affinity);
                    affinityMatrix.iRow.push_back(j+1+i*mat.cols);//index base 0
                }
            }    

            if( i+1 < mat.rows ) {
                affinity = affinityFunction(mat.at<double>(i,j),mat.at<double>(i+1,j), sigma); 
                if(affinity != 0.0) {                         
                    affinityMatrix.val.push_back(affinity);
                    affinityMatrix.iRow.push_back(j+(i+1)*mat.cols); //index base 0                                  
                }
            }                                 
        }
    }
    affinityMatrix.pCol.push_back(affinityMatrix.val.size());    
    printf("[OK]\n");
    return affinityMatrix;
}

double** get2DDoubleAffinityMatrix(Mat mat, double sigma) {
    /*
        Returns image affinity matrix 2D double** array [mat.rows * mat.cols][mat.rows * mat.cols] - not suitabla for large inputs, only for experimental purposes
        Affinity defined at double affinityFunction(double val1, double val2)
        4 neigbour pixels
    */

    int numberOfPixels = mat.rows * mat.cols;
    double** affinityMatrix = new double*[numberOfPixels];
    for(int i = 0; i < numberOfPixels; i++) {
        affinityMatrix[i] = new double[numberOfPixels];
    }
    
    for(int i = 0; i < numberOfPixels; i++) {
        for(int j = 0; j < numberOfPixels; j++) {
            affinityMatrix[i][j] = 0.0f;
        }
    }

    double affinity;
    for(int i = 0; i < mat.rows; i++) {
        for(int j = 0; j < mat.cols; j++) {
            if(j+1 < mat.cols) {
                affinity = affinityFunction(mat.at<double>(i,j), mat.at<double>(i, j+1), sigma);

                //printf("%f %f  aff:%f\n",mat.at<double>(i,j),mat.at<double>(i,j+1),affinity);
                if(affinity != 0.0) { 
                    affinityMatrix[j+i*mat.cols][j+1+i*mat.cols] = affinity;    //UPPER PART OF AFFINITY MATRIX
                    affinityMatrix[j+1+i*mat.cols][j+i*mat.cols] = affinity; //LOWER PART OF AFFINITY MATRIX
                }
            }

            if(i+1 < mat.rows) {
                affinity = affinityFunction(mat.at<double>(i,j),mat.at<double>(i+1,j), sigma); 
                //printf("%f %f  aff:%f\n",mat.at<double>(i,j),mat.at<double>(i+1,j),affinity);
                if(affinity != 0.0)
                {  
                    affinityMatrix[j+i*mat.cols][j+(i+1)*mat.cols] = affinity;  //UPPER PART OF AFFINITY MATRIX
                    affinityMatrix[j+(i+1)*mat.cols][j+i*mat.cols] = affinity; //LOWER PART OF AFFINITY MATRIX
                }
            }
        }
    }    
    return affinityMatrix;
}

void print2DDoubleArray(double** arr, int rows, int cols) {
    /*
        Prints 2D double** array to std output
    */
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            if(arr[i][j] != 0.0)
                printf( GREEN "%.1f " RESETCOLOR, arr[i][j]);
            else
                printf("%.1f ", arr[i][j]);
        }
        printf("\n");
    }

    //wolfram alpha format
    /*printf("[");
    for(int i = 0; i < rows; i++)
    {
        printf("[");
        for(int j = 0; j < cols; j++)
        {
                printf( GREEN "%.0f" RESETCOLOR, arr[i][j]);

                if(j < cols-1)
                    printf(",");
            
        }
        printf("]");
        if(i < rows-1)
            printf(",");
    }
    printf("]");*/
}

void save2DDoubleArray(char* filename, double** arr, int rows, int cols) {
    FILE* f = fopen(filename,"w");
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
                fprintf(f, "%.1f ", arr[i][j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}
