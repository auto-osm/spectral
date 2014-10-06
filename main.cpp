#include "main.h"


Mat loadDoubleImage(const char* filename)
{
    /*
    Returns double<0,1> Mat object loaded from the file
    */
    Mat  imageUchar, imageDouble;
    imageUchar = imread(filename, 0);  
    imageUchar.convertTo(imageDouble, CV_64F, 1/255.0);
    //imshow("Src Image", imagedouble);

    printf("Image loaded %d x %d (w x h)\n",imageDouble.cols,imageDouble.rows);      
    return imageDouble;
}

void printMatdouble(Mat img)
{
    /*
    Prints Mat double object values to std output
    */

    for(int i = 0; i < img.rows; i++){
        for(int j = 0; j < img.cols; j++){
            printf("%.2f ", img.at<double>(i,j));
        }
        printf("\n");
    }
}


void diffuse(vector<vector<double > > &data, double *eigVal)
{
    /*
        DRAFT
    */


    double alpha = 10000.0;
    printf("Using diffuse coords...\n");
    for(size_t i = 0; i < data.size();i++)
    {
        for(size_t j = 0; j < data[i].size(); j++ )
        {
            //printf("%f * %f\n",data[i][j], eigVal[j]);
            data[i][j] = data[i][j]*exp(-1.0 * eigVal[j] * alpha); 
        }
    }
}


vector<vector<double > > getDataPoints(double* eigVec, int nev, int vecLen)
{
    /*
  
        Create data points from k eigenvectors x stacked in to the COLUMNS M=[x0,...,xk]
        Data points created from the rows of M and normalized ex dataPoint[0] - first k-dimensional point for clustering
        
    */
    vector<vector<double > > dataPoints(vecLen, vector<double> (nev));

    for(int i = 0; i < nev; i++)
    {
        for(int j = 0; j<vecLen; j++)
        {
            dataPoints[j][i] = eigVec[j+i*vecLen];
        }
    }
    

    return dataPoints;
}


vector<vector<double > > getDataPointsTrans(double* eigVec, int nev, int vecLen)
{
    /*
        Create data points from k eigenvectors x stacked in to the ROWS       
    */
    vector<vector<double > > dataPoints(nev, vector<double> (vecLen));

    for(int i = 0; i < nev; i++)
    {
        for(int j = 0; j<vecLen; j++)
        {
            dataPoints[i][j] = eigVec[j+i*vecLen];
        }
    }

    return dataPoints;
}


vector<vector<double > > spectralDecomposition(CSCMat mat, int nev)
{
    /*

    Compute Eigenvectors and Eigenvalues of lower Laplacian matrix 

    WHAT IS PROBLEM DIMENSION?
    WHAT IS MAXIMUM NUMBER EIGEN VALUES ?!?!

    */
    
    int n = mat.pCol.size() -1;           // dimension (number of rows or cols od similiar matrix)
    //int nev = 3;      //number of requested eigenvectors/eigenvalues
    int nconv;       // number of "converged" eigenvalues.
    int nnz = mat.val.size();
    
    double* eigVal = new double[n];  // Eigenvalues.
    double* eigVec = new double[n*nev]; // Eigenvectors stored sequentially.    

    char uplo = 'L';
    
    nconv = AREig(eigVal, eigVec, n, nnz, &mat.val[0], &mat.iRow[0], &mat.pCol[0], uplo, nev, "SM", 0, 0.0, 1000000);

    Solution(nconv, n, nnz, &mat.val[0], &mat.iRow[0], &mat.pCol[0], uplo, eigVal, eigVec);
  
    
    vector<vector<double > > dataPoints = getDataPoints(eigVec,nev,n); // unnormalized data point for clustering eigenvectors in columns

    vector<vector<double > > dataPointsTrans = getDataPointsTrans(eigVec,nev,n);
    
    //diffuse(dataPoints,eigVal);

    //print2DVecArray(dataPoints);

    vecNormalize(dataPoints);
    
    return dataPoints;
}



int main(int argc, char** argv)
{
    int nev = atoi(argv[2]); // number of required EigenVal/Vecs

    printf("nev= %d\n",nev);

    Mat sourceImage; 
    Mat segmentedImage; 

   //for(int f = 2; f < 15; f++)
    //{
        sourceImage = loadDoubleImage(argv[1]);
        segmentedImage = Mat::zeros(sourceImage.size(),sourceImage.type());
        //printMatdouble(sourceImage);    

        //double** affMatrix = get2DdoubleAffinityMatrix(sourceImage);
        //print2DdoubleArray(affMatrix, sourceImage.rows*sourceImage.cols,sourceImage.rows*sourceImage.cols);


        CSCMat affinityMat = getCSCAffinityMatrix(sourceImage);
        //printCSCMatrix(affinityMat);

        CSCMat laplacianMat = getCSCNormalizedLaplacianMatrix(affinityMat);
        //printCSCMatrix(laplacianMat);
        
        
        vector<vector<double > > dataPoints = spectralDecomposition(laplacianMat,nev);
        //print2DVecArray(dataPoints);


        //vector<int > clusters = k_means(dataPoints,k);
        vector<int > clusters = meanshiftFast(dataPoints,sourceImage.rows,sourceImage.cols);

        
        for(int i = 0; i<segmentedImage.rows; i++)
        {
            for(int j = 0; j< segmentedImage.cols; j++)
            {       
                segmentedImage.at<double>(i,j) = ((clusters[j+i*segmentedImage.cols]*40)%256)/255.0;
            }
        }

        //char str[15];
        //sprintf(str, "%s%d.png", argv[1],f);


        imshow("Source Image", sourceImage);
        imshow("Segmented Image", segmentedImage);
        segmentedImage.convertTo(segmentedImage, CV_8U, 255.0);


        

        imwrite(./out.png,segmentedImage);
    //}

    printf("Done!\n");
    waitKey(0);
	return 0;
  
}
