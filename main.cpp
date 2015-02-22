#include "main.h"
#define DEBUG

Mat loadDoubleImage(const char* filename) {
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

void printMatdouble(Mat img) {
    /*
        Prints Mat double object values to std output
    */
    for(int i = 0; i < img.rows; i++) {
        for(int j = 0; j < img.cols; j++) {
            printf("%.2f ", img.at<double>(i,j));
        }
        printf("\n");
    }
}

void diffuse(vector<vector<double > > &data, double *eigVal, double t) {
    /*
        Creates diffuse map
    */
    printf("Using diffuse coords...\n");
    for(size_t i = 0; i < data.size();i++) {
        for(size_t j = 0; j < data[i].size(); j++ ) {
            //printf("%f * %f\n",data[i][j], eigVal[j]);
            data[i][j] = data[i][j]*exp(-1.0 * eigVal[j] * t); 
        }
    }
}

vector<vector<double > > getDataPoints(double* eigVec, int nev, int vecLen) {
    /*
        Create data points from k eigenvectors u stacked in to the COLUMNS M=[u1,...,uk]
        Data points created from the rows of M and normalized ex dataPoint[0] - first k-dimensional point for clustering    
    */
    vector<vector<double > > dataPoints(vecLen, vector<double> (nev));

    for(int i = 0; i < nev; i++) {
        for(int j = 0; j<vecLen; j++) {
            dataPoints[j][i] = eigVec[j+i*vecLen];
        }
    } 
    return dataPoints;
}

vector<vector<double > > getDataPointsTrans(double* eigVec, int nev, int vecLen) {
    /*
        Create data points from k eigenvectors u stacked in to the ROWS       
    */
    vector<vector<double > > dataPoints(nev, vector<double> (vecLen));

    for(int i = 0; i < nev; i++) {
        for(int j = 0; j<vecLen; j++) {
            dataPoints[i][j] = eigVec[j+i*vecLen];
        }
    }
    return dataPoints;
}

vector<vector<double > > spectralDecomposition(CSCMat mat, int nev) {
    /*
        Compute Eigenvectors and Eigenvalues of lower Laplacian matrix 
    */    
    int n = mat.pCol.size() -1;           // dimension (number of rows or cols od similiar matrix)
    int nconv;       // number of "converged" eigenvalues.
    int nnz = mat.val.size();
    
    double* eigVal = new double[n];  // Eigenvalues.
    double* eigVec = new double[n*nev]; // Eigenvectors stored sequentially.    

    char uplo = 'L';
    
    #ifdef DEBUG
        printf("Spectral decomposition started \n");
	unsigned int start_time = time(NULL);
    #endif

    nconv = AREig(eigVal, eigVec, n, nnz, &mat.val[0], &mat.iRow[0], &mat.pCol[0], uplo, nev, "SM", 0, 0.0, 1000000);

    #ifdef DEBUG
        unsigned int end_time = time(NULL);
        printf("Spectral decomposition finished in %u s\n", end_time-start_time);
    #endif
    
    Solution(nconv, n, nnz, &mat.val[0], &mat.iRow[0], &mat.pCol[0], uplo, eigVal, eigVec);
 
    vector<vector<double > > dataPoints = getDataPoints(eigVec,nev,n); // unnormalized data point for clustering eigenvectors in columns
    //vector<vector<double > > dataPointsTrans = getDataPointsTrans(eigVec,nev,n);
    
    //diffuse(dataPoints,eigVal);

    //print2DVecArray(dataPoints);

    vecSave2DArray("eigvec.txt", dataPoints);
    vecNormalize(dataPoints);
    vecSave2DArray("eigvecnorm.txt", dataPoints);
    return dataPoints;
}

int main(int argc, char** argv) {
    char* input_file_path = argv[1];

    int max_nev = atoi(argv[2]); // number of required EigenVal/Vecs

    double sigma_affmat = 0.05;
    double sigma_meanshift = 0.05;

    Mat sourceImage; 
    Mat segmentedImage;

    time_t start_time;
    time_t current_time;
    time_t end_time;

    char output_filename[100];
    FILE* log = fopen("./log", "a");
    for(int nev = 2; nev < max_nev; nev++) {
        start_time = time(NULL);
        fprintf(log,"[%lu-%s]\nnev=%d, sigma_affmat=%f, sigma_meanshift=%f", start_time, basename(input_file_path), nev, sigma_affmat, sigma_meanshift);
        char output_filename[100];
        sprintf(output_filename, "./out/%lu-%s", start_time, basename(input_file_path));
        printf("%s\n", output_filename);

        sourceImage = loadDoubleImage(input_file_path);
        segmentedImage = Mat::zeros(sourceImage.size(), sourceImage.type());
        //printMatdouble(sourceImage);    

        double** affMatrix = get2DDoubleAffinityMatrix(sourceImage, sigma_affmat);
        save2DDoubleArray("affmat.txt", affMatrix, sourceImage.rows*sourceImage.cols, sourceImage.rows*sourceImage.cols);

        CSCMat affinityMat = getCSCAffinityMatrix(sourceImage, sigma_affmat);
        printCSCMatrix(affinityMat);
        CSCMat laplacianMat = getCSCLaplacianRW(affinityMat);
        printCSCMatrix(laplacianMat);
        current_time = time(NULL);
        vector<vector<double > > dataPoints = spectralDecomposition(laplacianMat, nev);
        fprintf(log,"spectral decomposition time: %lu\n", time(NULL)-current_time);
        //print2DVecArray(dataPoints);

        //vector<int > clusters = k_means(dataPoints,k);
        current_time = time(NULL);
        vector<int > clusters = meanshiftFast(dataPoints, sourceImage.rows, sourceImage.cols, sigma_meanshift); 
        fprintf(log,"clustering time: %lu\n", time(NULL)-current_time);

        for(int i = 0; i < segmentedImage.rows; i++) {
            for(int j = 0; j < segmentedImage.cols; j++) {       
                segmentedImage.at<double>(i,j) = ((clusters[j+i*segmentedImage.cols]*40)%256)/255.0;
            }
        }

        //char str[15];
        //sprintf(str, "%s%d.png", argv[1],f);
      
        //imshow("Source Image", sourceImage);
        //imshow("Segmented Image", segmentedImage);
        segmentedImage.convertTo(segmentedImage, CV_8U, 255.0);
        fprintf(log, "Total time: %lu\n\n", time(NULL) - start_time);
        imwrite(output_filename, segmentedImage);
        sleep(1);
    }

    fclose(log);
    printf("Done!\n");
    waitKey(0);
	return 0;
}
