CPP = g++

OPENCV_LIBS = -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_features2d -lopencv_video -lopencv_objdetect -lopencv_calib3d -lopencv_flann -lopencv_contrib -lopencv_legacy -lsuperlu -lblas -llapack -lgfortranbegin -lnsl

SPECTRAL_LIBS = -larpack -llapack -lsuperlu -lblas -lgfortran 

ALL_LIBS = $(OPENCV_LIBS) $(SPECTRAL_LIBS)

all: main

main: libs
	$(CPP) -o scisa *.o $(ALL_LIBS)

libs:	main.cpp meanshift.cpp vecops.cpp laplacian.cpp affinity.cpp math.cpp cscmatrix.cpp
	$(CPP) *.cpp -c

.PHONY:	clean
clean:
	rm -f *~ *.o core main
