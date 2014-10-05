#ifndef _STDAFX_H_
#define _STDAFX_H_

#define GREEN  "\033[22;31m"
#define RESETCOLOR "\033[0m"

#include <stdio.h>
#include <math.h>
#include <vector>
#include <cstdlib>
#include <time.h>


#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>


#include "/home/amic/Dropbox/arpack++/include/areig.h"
#include "/home/amic/Dropbox/arpack++/include/asymsol.h"


using namespace cv;
using namespace std;

#include "cscmatrix.h"
#include "math.h"
#include "vecops.h"
#include "spectral.h"
#include "meanshift.h"
#include "kmeans.h"
#include "affinity.h"
#include "laplacian.h"


#endif
