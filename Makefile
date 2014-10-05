

PLAT         = linux
CPP          = g++


ARPACK_LIB   = -larpack
LAPACK_LIB   = -llapack
UMFPACK_LIB  = #-lumfpack
SUPERLU_LIB  = -lsuperlu
BLAS_LIB     = -lblas
FORTRAN_LIBS = -lgfortran

# Defining g++ flags and directories.

# CPP_WARNINGS = -fpermissive 
CPP_WARNINGS = -Wall -ansi -pedantic-errors
CPP_DEBUG    = -g
CPP_OPTIM    = -O
CPP_LIBS     = 
CPP_INC      = 

#CPP_FLAGS    = $(CPP_DEBUG) -D$(PLAT) -I$(ARPACKPP_INC) -I$(CPP_INC) $(CPP_WARNINGS)


OPENCV_LIBS = -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_features2d -lopencv_video -lopencv_objdetect -lopencv_calib3d -lopencv_flann -lopencv_contrib -lopencv_legacy -lsuperlu -lblas -llapack -lgfortranbegin -lnsl


ALL_LIBS     = $(CPP_LIBS) $(ARPACK_LIB) $(BLAS_LIB) $(LAPACK_LIB) $(FORTRAN_LIBS) $(OPENCV_LIBS) $(SUPERLU_LIB)


Debug: all
all: main

# compiling and linking each symmetric problem.


main:	main.o
	$(CPP) -o main *.o  $(ALL_LIBS)

main.o:	main.cpp
	$(CPP) *.cpp -c #-o main.o $(CPP_FLAGS) $(FLAGS) -I$(CSCMAT_DIR) -I$(SUPERLU_DIR)


# defining cleaning rule.

.PHONY:	clean
clean:
	rm -f *~ *.o core main


