#include "FUF.h"

#include <math.h>
#include <stdio.h>
#include "Utils.h"


void modifiedDiscreteCosineTransform(const int* x, int size, int* z);
void inverseModifiedDiscreteCosineTransform(const int* z, int size, int* x);


void FUF::transformCompress() {
    WaveData* data = &(sample.data);
    int cn = data->channelCount;
    int dn = data->dataLength;

    int** zdata = new int*[cn];
    for (int c = 0; c < cn; c++) {
        zdata[c] = new int[dn/2];
        modifiedDiscreteCosineTransform(data->data[c], dn, zdata[c]);
    }
}

void FUF::transformDecompress() {
    WaveData* data = &(sample.data);
    int cn = data->channelCount;
    int dn = data->dataLength;

    int** zdata = new int*[cn];
    for (int c = 0; c < cn; c++) {
        zdata[c] = new int[dn/2];
        inverseModifiedDiscreteCosineTransform(zdata[c], dn, data->data[c]);
    }
}


void modifiedDiscreteCosineTransform(const int* x, int size, int* z) {
    double* s = new double[size/2];
    for (int i = 0; i < size; i++) {
        s[i] = 0;
    }

    for (int i = 0; i < size/2; i++) {
        for (int k = 0; k < size; k++) {
            s[i] += x[k] * cos( M_PI / (2*size) * (2*k + 1 + size/2) * (2*i + 1) );
        }
    }

    for (int i = 0; i < size; i++) {
        z[i] = (int) s[i];
    }

    delete[] s;
}


void inverseModifiedDiscreteCosineTransform(const int* z, int size, int* x) {
    double* xk = new double[size];
    for (int i = 0; i < size; i++) {
        xk[i] = 0;
    }

    for (int k = 0; k < size; k++) {
        for (int i = 0; i < size/2; i++) {
            xk[k] += z[i] * cos( M_PI / (2*size) * (2*k + 1 + size/2) * (2*i + 1) );
        }
    }

    for (int i = 0; i < size; i++) {
        x[i] = (int) xk[i];
    }

    delete[] xk;
}

