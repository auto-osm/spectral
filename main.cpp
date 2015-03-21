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
    printf("Using diffuse coords...");
    for(size_t i = 0; i < data.size();i++) {
        for(size_t j = 0; j < data[i].size(); j++ ) {
            //printf("%f * %f\n",data[i][j], eigVal[j]);
            data[i][j] = data[i][j]*exp(-1.0 * eigVal[j] * t); 
        }
    }
    printf("done!\n");

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

vector<vector<double > > spectralDecomposition(CSCMat mat, int nev, double diffuse_t) {
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
    
    diffuse(dataPoints, eigVal, diffuse_t);

    //print2DVecArray(dataPoints);

    //vecSave2DArray("eigvec.txt", dataPoints);
    vecNormalize(dataPoints);
    //vecSave2DArray("eigvecnorm.txt", dataPoints);
    return dataPoints;
}

void visualiseSegments(Mat &output_img, vector<vector<int > > &clusters) {
    for(int i = 0; i < clusters.size(); i++) {
            Vec3b color(rand()%256, rand()%256, rand()%256);
            for(int j = 0; j < clusters[i].size(); j++) {
                int index = clusters[i][j];
                int r = (int) (index / output_img.cols);  //row
                int c = index % output_img.cols; //column
                output_img.at<Vec3b>(r,c) = color;
        }
    }
}

int main(int argc, char** argv) {
    double sigma_affmat = 0.03;
    double sigma_meanshift;
    double diffuse_t = 0.0;
    double t_arr[] = {1000.0,10000.0};
    FILE* log = fopen("./log-synimg-noise", "a");
    for(int t = 0; t < 2; t++) {
    diffuse_t =  t_arr[t];
    double sigma_affmat = 0.03;
    for(int sa = 0; sa < 3; sa++) {
    sigma_meanshift = 0.1; 
    for(int sm = 0; sm < 4; sm++) { 
    char* input_file_path = argv[1];

    int max_nev = atoi(argv[2]); // number of required EigenVal/Vecs


    Mat source_img; 
    Mat output_img;

    time_t start_time;
    time_t current_time;
    time_t end_time;

    char output_filename[100];
    for(int nev = 2; nev <= max_nev; nev++) {
        start_time = time(NULL);
        fprintf(log,"[%lu-%s]\nnev=%d, sigma_affmat=%f, sigma_meanshift=%f, diffuse_t=%f", start_time, basename(input_file_path), nev, sigma_affmat, sigma_meanshift, diffuse_t);
        char output_filename[100];
        sprintf(output_filename, "./out/synt_noise/%lu_sa%d_sm%d_nev%d_difft%d-%s", start_time, (int)(sigma_affmat*1000),(int)(sigma_meanshift*1000), nev,(int)diffuse_t, basename(input_file_path));
        printf("%s\n", output_filename);

        source_img = loadDoubleImage(input_file_path);
        output_img = Mat::zeros(source_img.size(), CV_8UC3);
        //printMatdouble(source_img);    

        //double** affMatrix = get2DDoubleAffinityMatrix(source_img, sigma_affmat);
        //save2DDoubleArray("affmat.txt", affMatrix, source_img.rows*source_img.cols, source_img.rows*source_img.cols);

        CSCMat affinityMat = getCSCAffinityMatrix(source_img, sigma_affmat);
        //printCSCMatrix(affinityMat);

        CSCMat laplacianMat = getCSCLaplacianSym(affinityMat);
        //printCSCMatrix(laplacianMat);

        current_time = time(NULL);
        vector<vector<double > > dataPoints = spectralDecomposition(laplacianMat, nev, diffuse_t);
        fprintf(log,"spectral decomposition time: %lu\n", time(NULL) - current_time);
        //print2DVecArray(dataPoints);

        //vector<int > clusters = k_means(dataPoints,k);
        current_time = time(NULL);
        //vector<int > clusters = meanshiftFast(dataPoints, source_img.rows, source_img.cols, sigma_meanshift); 
        vector<vector<double > > datameans = meanshift(dataPoints, sigma_meanshift); 
        //vecPrint2DArray(datameans);
        //printf("%d\n",datameans.size());
        fprintf(log,"clustering time: %lu\n", time(NULL)-current_time);
        //vector<int> clusters = cluster(datameans, 1.0);        
        vector<vector<int > > clusters = cluster2(datameans, 1.0);        
        //printf("%d\n",clusters.size());
        /*for(int i = 0; i < segmentedImage.rows; i++) {
            for(int j = 0; j < segmentedImage.cols; j++) {       
                segmentedImage.at<double>(i,j) = ((clusters[j+i*segmentedImage.cols]*40)%256)/255.0;
            }
        }*/

        
        visualiseSegments(output_img, clusters);
        //char str[15];
        //sprintf(str, "%s%d.png", argv[1],f);
      
        //imshow("Source Image", source_img);
        //imshow("Segmented Image", segmentedImage);
        

        //segmentedImage.convertTo(segmentedImage, CV_8U, 255.0);
        fprintf(log, "Total time: %lu\n\n", time(NULL) - start_time);
        imwrite(output_filename, output_img);
        sleep(1);
    }
    sigma_meanshift += 0.1;
    }
    sigma_affmat += 0.01;
    }
    }

    fclose(log);
    printf("Done!\n");
    waitKey(0);
	return 0;
}
