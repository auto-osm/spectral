
#include "math.h"

double gaussianFunction(double val, double sigma)
{
    return exp(-1*sqr(val)/(2*sqr(sigma)));
}
