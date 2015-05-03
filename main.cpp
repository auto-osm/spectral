#include "main.h"
#define DEBUG

Mat loadDoubleImage(const char* filename) {
    /*
        Returns double<0,1> Mat object loaded from the file
    */
    Mat imageUchar, imageDouble;
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

void arg_error() {
    printf("Use following syntax to run algorithm:\n scisa -f file_path [-sm val -sa val -t val -n val]\n");
    exit(-1);
}

int main(int argc, char** argv) {
    FILE* log = fopen("./logs/log", "a");
    //default configuration
    char* input_file_path;
    char output_filename[100];
    double sigma_meanshift = 0.4;
    double sigma_affmat = 0.005;
    double diffuse_t = 5000.0;
    int nev = 20;

    Mat source_img; 
    Mat output_img;

    time_t start_time;
    time_t current_time;
    time_t end_time;

    //parse command line arguments
    for(int i = 0; i < argc; i++) {
        if(strcmp(argv[i],"-f")==0) {
	        i++;
	        input_file_path = argv[i];
            printf("-f: %s\n", input_file_path);
	    } 
        if(strcmp(argv[i],"-sa")==0) {
	        i++;
	        sigma_affmat = atof(argv[i]);
            printf("-sa: %f\n", sigma_affmat);
	    }
        if(strcmp(argv[i],"-sm")==0) {
	        i++;
	        sigma_meanshift = atof(argv[i]);
            printf("-sm: %f\n", sigma_meanshift);
	    }
        if(strcmp(argv[i],"-t")==0) {
	        i++;
	        diffuse_t = atof(argv[i]);
            printf("-t: %f\n", diffuse_t);
	    }
	    if(strcmp(argv[i],"-n")==0) {
	        i++;
	        nev = atoi(argv[i]);
            printf("-n: %d\n", nev);
	    }
    }
    if(access(input_file_path, F_OK) == -1) {
        printf("Invalid input file path: %s\n", input_file_path);
        arg_error();
    }

    printf("Args parsing completed!\n");
    printf("%s %s -sa %f -sm %f -t %f -n %d\n", argv[0], input_file_path, sigma_affmat, sigma_meanshift, diffuse_t, nev);

    start_time = time(NULL);

    /* Load image data */
    source_img = loadDoubleImage(input_file_path);
    output_img = Mat::zeros(source_img.size(), CV_8UC3);
    //printMatdouble(source_img);    


    /* Log record */
    fprintf(log,"[%lu-%s]\nnev=%d, sigma_affmat=%f, sigma_meanshift=%f, diffuse_t=%f", start_time, basename(input_file_path), nev, sigma_affmat, sigma_meanshift, diffuse_t);
    sprintf(output_filename, "./out/%lu_sa%d_sm%d_nev%d_difft%d-%s", start_time, (int)(sigma_affmat*1000),(int)(sigma_meanshift*1000), nev,(int)diffuse_t, basename(input_file_path));

    /* Compute affinity matrix */
    CSCMat affinityMat = getCSCAffinityMatrix(source_img, sigma_affmat);
    //printCSCMatrix(affinityMat);

    /* Compute laplacian matrix */
    CSCMat laplacianMat = getCSCLaplacianSym(affinityMat);
    //printCSCMatrix(laplacianMat);

    current_time = time(NULL);
    
    /* Compute eigen vectors*/
    vector<vector<double > > dataPoints = spectralDecomposition(laplacianMat, nev, diffuse_t);
    //print2DVecArray(dataPoints);

    /* Log record */
    fprintf(log,"Spectral decomposition time: %lu\n", time(NULL) - current_time);

    current_time = time(NULL);
    
    /*Compute clusters*/
    vector<vector<double > > datameans = meanshift(dataPoints, sigma_meanshift); 
    //vecPrint2DArray(datameans);
    
    /* Log record */
    fprintf(log,"Clustering time: %lu\n", time(NULL) - current_time);
    vector<vector<int > > clusters = cluster2(datameans, 1.0);

    /* Colorize segments*/    
    visualiseSegments(output_img, clusters);
    
    /* Log record */
    fprintf(log, "Total time: %lu\n\n", time(NULL) - start_time);

    imwrite(output_filename, output_img);

    fclose(log);
    printf("Done!\n");
    waitKey(0);
	return 0;
}
