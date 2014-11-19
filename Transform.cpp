#include "FUF.h"

#include <math.h>
#include <stdio.h>
#include "Utils.h"


void discreteCosineTransform(const int* x, int size, int* z);
void inverseDiscreteCosineTransform(const int* z, int size, int* x);


void FUF::transformCompress() {
    WaveData* data = &(sample.data);
    int cn = data->channelCount;
    int dn = data->dataLength;

    for (int c = 0; c < cn; c++) {
        discreteCosineTransform(data->data[c], dn, data->data[c]);
    }
}

void FUF::transformDecompress() {
    WaveData* data = &(sample.data);
    int cn = data->channelCount;
    int dn = data->dataLength;

    for (int c = 0; c < cn; c++) {
        inverseDiscreteCosineTransform(data->data[c], dn, data->data[c]);
    }
}


void discreteCosineTransform(const int* x, int size, int* z) {
    double** xk = new double*[size];

    for (int n = 0; n < size; n++) {
        xk[n] = new double[size];
    }

    for (int n = 0; n < size; n++) {
        xk[0][n] = 1/sqrt(size) * x[n];
    }

    for (int k = 1; k < size; k++) {
        for (int n = 0; n < size; n++) {
            xk[k][n] = sqrt(2.0/size) * cos(k*M_PI/(2.0*size)*(2.0*n+1)) * x[n];
        }
    }

    double* temp = new double[size];
    for (int k = 0; k < size; k++) {
        temp[k] = 0;
    }

    for (int k = 0; k < size; k++) {
        for (int n = 0; n < size; n++) {
            temp[k] += xk[k][n];
        }
    }

    for (int k = 0; k < size; k++) {
        z[k] = (int) temp[k];
        delete[] xk[k];
    }

    delete[] temp;
    delete[] xk;
}


void inverseDiscreteCosineTransform(const int* z, int size, int* x) {
    double** xk = new double*[size];

    for (int n = 0; n < size; n++) {
        xk[n] = new double[size];
    }

    for (int n = 0; n < size; n++) {
        xk[0][n] = sqrt(1.0/size) * z[0];
    }

    for (int k = 1; k < size; k++) {
        for (int n = 0; n < size; n++) {
            xk[k][n] = sqrt(2.0/size) * z[n] * cos(k*(2.0*n+1)*M_PI/(2.0*size));
        }
    }

    double* temp = new double[size];
    for (int k = 0; k < size; k++) {
        temp[k] = 0;
    }

    for (int k = 0; k < size; k++) {
        for (int n = 0; n < size; n++) {
            temp[k] += xk[n][k];
        }
    }

    for (int k = 0; k < size; k++) {
        x[k] = (int) temp[k];
        delete[] xk[k];
    }

    delete[] temp;
    delete[] xk;
}

