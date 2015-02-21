#include "meanshift.h"
#define DEBUG_MEANSHIFT

double meanshiftKernelFunction(vector<double > &v1, vector<double > &v2, double sigma) {
    double dist = vecEuclidDist(v1,v2);
    double res = gaussianFunction(dist,sigma);
    return res;
}

vector<int> meanshift(vector<vector<double > > &data, double sigma) {    
    int maxIter = 20;
    int iter;
    vector<int> clusters(data.size(),0);

    vector<vector<double > > oldMeans = data;
    vector<vector<double > > newMeans(data.size(),vector<double >(data[0].size(),0.0));

    double treshold = 0.001;
    double distance;
  
    double weight;
    double sumWeight;
    printf("Meanshift clustering ...");
     
    for(size_t i = 0; i< oldMeans.size();i++) {
        iter = 0;
        //printf("\n");
        do { 
            sumWeight = 0.0;
            weight = 0.0;
            fill(newMeans[i].begin(), newMeans[i].end(), 0.0);
            
            for(size_t j = 0; j < data.size(); j++) {
                if((weight = meanshiftKernelFunction(oldMeans[i], data[j], sigma)) != 0.0) {
                    vector<double> scaledVec = vecGetScale(data[j], weight);
                    vecSum(newMeans[i],scaledVec);
                    sumWeight += weight;
                }
            }

            vecDiv(newMeans[i],sumWeight); //deleni vektory

            distance = vecEuclidDist(newMeans[i], oldMeans[i]);

            oldMeans[i] = newMeans[i];
            iter++;
            //printf("#");
        }while(distance > treshold && iter < maxIter);//zastaveni
    }

//print2DVecArray(oldMeans);
    vector<vector<double > > means;
    double segTreshold = 1.0;//0.6*sqrt(oldMeans[0].size());
    double distMin = 1000.0;
    bool found = false;    
    for(size_t i = 0; i < oldMeans.size(); i++) {
        found = false;
        distMin=1000.0;
        for(size_t j = 0; j < means.size(); j++) {
            distance = vecEuclidDist(oldMeans[i], means[j]);
            
            //printf("%f ", distance);
            if(distance < segTreshold && distance < distMin) { 
                found = true;
                clusters[i] = j;
                distMin = distance;
            }

        }        
        if(!found) {
            means.push_back(oldMeans[i]);
            clusters[i] = means.size()-1;
        }
    }

    printf("Segmented into %d clusters..\n",(int)means.size());

    return clusters;
}


vector<int> meanshiftFast(vector<vector<double > > &data, int imgHeight, int imgWidth, double sigma) {
    int windowRadius = 5;
    
    int maxIter = 20;
    int iter;
    vector<int> clusters(data.size(),0);

    vector<vector<double > > oldMeans = data;
    vector<vector<double > > newMeans(data.size(),vector<double >(data[0].size(),0.0));

    double treshold = 0.001;
    double distance;
  
    double weight;
    double sumWeight;
    printf("Meanshift clustering ...");
     
    for(int y = 0; y < imgHeight; y++) {
        for(int x = 0; x < imgWidth; x++) {
            iter = 0;
            //printf("\n");
            do { 
                sumWeight = 0.0;
                weight = 0.0;
                fill(newMeans[x+y*imgWidth].begin(), newMeans[x+y*imgWidth].end(), 0.0);
                
                for(int wy = y - windowRadius; wy <= y+windowRadius; wy++) {
                    for(int wx = x - windowRadius; wx <= x+windowRadius; wx++) {   
                            if(wy >= 0 && wy < imgHeight && wx >= 0 && wx < imgWidth && vecEuclidDist(oldMeans[x+y*imgWidth],data[wx+wy*imgWidth]) <= windowRadius) {
                                if((weight = meanshiftKernelFunction(oldMeans[x+y*imgWidth], data[wx+wy*imgWidth], sigma)) != 0.0) {
                                    vector<double> scaledVec = vecGetScale(data[wx+wy*imgWidth], weight);
                                    vecSum(newMeans[x+y*imgWidth],scaledVec);
                                    sumWeight += weight;
                                }
                            }
                    }
                }

                vecDiv(newMeans[x+y*imgWidth],sumWeight); //deleni vektory

                distance = vecEuclidDist(newMeans[x+y*imgWidth], oldMeans[x+y*imgWidth]);

                oldMeans[x+y*imgWidth] = newMeans[x+y*imgWidth];
                iter++;
                //printf("#");
            }while(distance > treshold && iter < maxIter);//zastaveni
        }
    }

//print2DVecArray(oldMeans);
    vector<vector<double > > means;
    double segTreshold = 1.0;//0.6*sqrt(oldMeans[0].size());
    double distMin = 1000.0;
    bool found = false;    
    for(size_t i = 0; i < oldMeans.size(); i++) {
        found = false;
        distMin=1000.0;
        for(size_t j = 0; j < means.size(); j++) {
            distance = vecEuclidDist(oldMeans[i], means[j]);
            
            //printf("%f ", distance);
            if(distance < segTreshold && distance < distMin) { 
                found = true;
                clusters[i] = j;
                distMin = distance;
            }

        }        
        if(!found) {
            means.push_back(oldMeans[i]);
            clusters[i] = means.size()-1;
        }
    }

    printf("Segmented into %d clusters..\n",(int)means.size());

    return clusters;
}



