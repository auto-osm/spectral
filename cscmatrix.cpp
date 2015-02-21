#include "cscmatrix.h"

void printCSCMatrix(CSCMat mat) {
    printf("\nvalues:\n");
    for(size_t i = 0; i < mat.val.size(); i++)
        printf("%.2f ",mat.val[i]);

    printf("\nrow index:\n");
    for(size_t i = 0; i < mat.iRow.size(); i++)
        printf("%d ",mat.iRow[i]);

    printf("\ncolumn pointer:\n");
    for(size_t i = 0; i < mat.pCol.size(); i++)
        printf("%d ",mat.pCol[i]);
    printf("\n");
}

