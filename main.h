#ifndef _MAIN_H_
#define _MAIN_H_

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <stdio.h>
using namespace cv;

#include "affinity.h"
#include "laplacian.h"
#include "meanshift.h"
#include "vecops.h"
#include "arpack++/include/areig.h"
#include "arpack++/include/asymsol.h"


#include <stdio.h>
#include <time.h>
#include <unistd.h>


Mat loaddoubleImage(const char* filename);

void printMatdouble(Mat img);

int main(int argc, char** argv);

#endif
