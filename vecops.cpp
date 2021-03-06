#include "vecops.h"

void vecSum(vector<double> &v1, vector<double> &v2) {
    /*
        Adds v2 to v1
        v1 = v1 + v2
        Modifies original vector v1!
    */

    size_t size = v1.size();

    for(size_t i = 0; i < size; i++) {
        v1[i] += v2[i];
    }
}


void vecDif(vector<double> &v1, vector<double> &v2) {
    /*
        Substracts v2 from v1       
        v1 = v1 - v2
        Modifies original vector v1!
    */

    size_t size = v1.size();

    for(size_t i = 0; i < size; i++) {
        v1[i] -= v2[i];
    }
}

void vecDiv(vector<double> &v1, double alpha) {
    /*
        Devide each element of v1 by alpha
        Modifies original vector v1!
    */
    size_t size = v1.size();

    for(size_t i = 0; i < size; i++) {
        v1[i] /= alpha;
    }
}

void vecScale(vector<double> &v1, double alpha) {
    /*
        Multiply each element of v1 by alpha
        Modifies original vector v1!     
    */

    size_t size = v1.size();

    for(size_t i = 0; i < size; i++) {
        v1[i] *= alpha;
    }
}

vector<double> vecGetScale(vector<double> &v1, double alpha) {
    /*
        Multiply each element of v1 by alpha
        Return new vector 
    */

    size_t size = v1.size();

    vector<double> res(size);

    for(size_t i = 0; i < size; i++) {
        res[i] = v1[i] * alpha;
    }
    return res;
}

double vecGetNorm(vector<double> &v1) {
    /*
        Returns vector norm (length)
    */

    double norm = 0.0;

    size_t size = v1.size();

    for(size_t i = 0; i < size; i++) {
        norm += sqr(v1[i]);
    }

    return sqrt(norm);
}


void vecNormalize(vector<double> &v1) {
    /*
        Normalize vector to unit lenght
        v[x] / ||v||
    */
    size_t size = v1.size();

    double norm = vecGetNorm(v1);

    for(size_t i = 0; i < size; i++) {
        v1[i] /= norm;
    }
}

void vecNormalize(vector<vector<double > > &v1) {
    for(size_t i = 0; i < v1.size(); i++) {
        vecNormalize(v1[i]);     
    }
}

double vecEuclidDist(vector<double> &v1, vector<double> &v2) {
    /*
        Returns distance of 2 n-dim points p1 stored in v1, p2 stored in v2

        distance = ||v1-v2||
    */
    
    size_t size = v1.size();    
    double dist = 0.0;
    
    for(size_t i = 0; i < size; i++) {
        dist += sqr(v1[i] - v2[i]);
    }
    return sqrt(dist);
}

void vecRandomize(vector<double> &v1) {
    for(size_t i = 0; i < v1.size(); i++) {
        v1[i] = ((rand() % 2000)/1000.0) - 1.0;
    }
    vecNormalize(v1);
}

void vecPrint(vector<double > &v) {
    for(size_t i = 0; i < v.size(); i++) {
        printf("%+.3f ", v[i]);
    }
    printf("\n");
}

void vecPrint(vector<int> &v) {
    for(size_t i = 0; i < v.size(); i++) {
        printf("%d ", v[i]);
    }
    printf("\n");
}

void vecPrint2DArray(vector<vector<double > > &vec2D) {  
    printf("Printing 2D vec<vec<double>> array...\n");
    for(size_t i = 0; i < vec2D.size() ; i++) {
        vecPrint(vec2D[i]);
    }
    printf("\n");
}

void vecSave(char* filename, vector<double > &v) {
    FILE* f = fopen("filename","w");
    for(size_t i = 0; i < v.size(); i++) {
        fprintf(f, "%+.3f ", v[i]);
    }
    fprintf(f,"\n");
    fclose(f);
}

void vecSave(FILE* f, vector<double > &v) {
    for(size_t i = 0; i < v.size(); i++) {
        fprintf(f, "%+.3f ", v[i]);
    }
    fprintf(f,"\n");
}

void vecSave2DArray(char* filename, vector<vector<double > > &vec2D) {  
    FILE* f = fopen(filename, "w");
    for(size_t i = 0; i < vec2D.size() ; i++) {
        vecSave(f, vec2D[i]);
    }
    fprintf(f,"\n");
    fclose(f);
}

double vecDotProduct(vector<double> &v1, vector<double> &v2) {
    double res = 0.0;
    for(size_t i = 0; i < v1.size(); i++ ) {
        res += v1[i]*v2[i];
    }
    return res;
}

void a(){}


