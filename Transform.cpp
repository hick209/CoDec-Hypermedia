#include "FUF.h"

#include <stdio.h>
#include "Utils.h"


void discreteCosineTransform(const int* x, int size, double* z);
void inverseDiscreteCosineTransform(const double* z, int size, double* x);


void FUF::transformCompress() {
    WaveData* data = &(sample.data);
    int cn = data->channelCount;
    int dn = data->dataLength;

    double z[cn][dn];

    for (int c = 0; c < cn; c++) {
        discreteCosineTransform(data->data[c], dn, z[c]);
    }
}


void discreteCosineTransform(const int* x, int size, double* z) {
    double xk[size][size];

    for (int n = 0; n < size; n++) {
        xk[0][n] = 1/sqrt(size) * x[n];
    }

    for (int k = 1; k < size; k++) {
        for (int n = 0; n < size; n++) {
            xk[k][n] = sqrt(2.0/size) * cos(k*M_PI/(2.0*size)*(2.0*n+1)) * x[n];
        }
    }

    for (int k = 0; k < size; k++) {
        z[k] = 0;
    }

    for (int k = 0; k < size; k++) {
        for (int n = 0; n < size; n++) {
            z[k] += xk[k][n];
        }
    }
}


void inverseDiscreteCosineTransform(const double* z, int size, double* x) {
    double xk[size][size];

    for (int n = 0; n < size; n++) {
        xk[0][n] = sqrt(1.0/size) * z[0];
    }

    for (int k = 1; k < size; k++) {
        for (int n = 0; n < size; n++) {
            xk[k][n] = sqrt(2.0/size) * z[n] * cos(k*(2.0*n+1)*M_PI/(2.0*size));
        }
    }

    for (int k = 0; k < size; k++) {
        x[k] = 0;
    }

    for (int k = 0; k < size; k++) {
        for (int n = 0; n < size; n++) {
            x[k] += xk[n][k];
        }
    }
}

