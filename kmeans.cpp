#include "kmeans.h"

vector<int> k_means(vector<vector<double > > dataPoints, int k)
{

    srand(time(0));

    vector<vector<double > > means(k,vector<double >(dataPoints[0].size()));
    vector<vector<double > > newMeans(k,vector<double >(dataPoints[0].size()));

    vector<int > clusters(dataPoints.size());
    vector<int > counters(k,0);

    
    //Generate k random means <-1, 1>
    for(size_t i = 0; i < means.size(); i++)
    {
        for(size_t j = 0; j < means[i].size(); j++)
        {   
            
            means[i][j] = ((rand() % 2000)/1000.0) - 1.0;
        }
        //vecNormalize(means[i]); /// Maybe??    
    }
    

    double minDist;
    double curDist;

    
    for(int iter=0; iter<10000; iter++)
    {

        //print2DVecArray(means);
        //find closest cluster for each data point
        for(size_t i = 0; i < dataPoints.size(); i++) // for each point
        {
             minDist = 1000.0;
             for(size_t j = 0; j < means.size(); j++)
             {
                curDist = vecEuclidDist(dataPoints[i],means[j]);
                
                //printf("from point %d to cluster %d = %f\n",i,j,curDist);
                if(curDist < minDist)
                {
                    minDist = curDist;
                    clusters[i] = j; //stores index of closest cluster
                } 
                
             }        
        }

        
        //reset counters and temp variables
        fill(counters.begin(), counters.end(), 0);
        for(size_t i = 0; i < means.size(); i++) 
        {
            fill(newMeans[i].begin(), newMeans[i].end(), 0.0);
        }


        //calculate new means
        for(size_t i = 0; i < dataPoints.size(); i++) // for each point
        {
            vecSum(newMeans[clusters[i]], dataPoints[i]);
            counters[clusters[i]]++;
        }

        for(size_t i = 0; i < means.size(); i++) 
        {
            if((double)counters[i] != 0) 
                vecDiv(newMeans[i],(double)counters[i]);
            else ///IMPORTANT TODO ACTION IF THERE IS NO POIN ASSIGNED TO MEAN - USING OLD COORD
                 vecRandomize(newMeans[i]); //Shuffle only unassigned mean
        }

        means = newMeans;

        
    }   

    //prints point - clusters assignement
/*    for(size_t i = 0; i < clusters.size(); i++) // for each point
    {
        if(i%(int)(sqrt((int)clusters.size())) == 0  ) //JEN PRO CTVERCOVE OBRAZY
            printf("\n");
        printf("%d ", clusters[i]);
    } 
   
    printf("\n");
*/
    return clusters;
}

