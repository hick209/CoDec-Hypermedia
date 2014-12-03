#include "FUF.h"

#include "Utils.h"
#include <math.h>
#include <iostream>

using namespace std;

void modifiedDiscreteCosineTransform(const int* x, int size, double* z);
void inverseModifiedDiscreteCosineTransform(const double* z, int size, int* x);


void FUF::transformCompress() {
    int cn = compressedData->getChannelCount();
    int dn = compressedData->getDataLength();

    compressedData->zData = new double*[cn];

    for (int c = 0; c < cn; c++) {
        compressedData->zData[c] = new double[dn/2];
    }

    for (int c = 0; c < cn; c++) {
        modifiedDiscreteCosineTransform(compressedData->getData(c), dn, compressedData->zData[c]);
    }
    compressedData->hasTransform = true;
}

void FUF::transformDecompress() {
    int cn = compressedData->getChannelCount();
    int dn = compressedData->getDataLength();

    for (int c = 0; c < cn; c++) {
        int* data = new int[dn * 2];
        inverseModifiedDiscreteCosineTransform(compressedData->zData[c], dn, data);
        compressedData->setData(c, data, dn*2);
    }
    compressedData->hasTransform = false;
}


void modifiedDiscreteCosineTransform(const int* x, int size, double* z) {
    for (int i = 0; i < size; i++) {
        z[i] = 0;
    }

    for (int i = 0; i < size/2; i++) {
        for (int k = 0; k < size; k++) {
            z[i] += x[k] * cos( M_PI / (2*size) * (2*k + 1 + size/2) * (2*i + 1) );
        }
    }
}


void inverseModifiedDiscreteCosineTransform(const double* z, int size, int* x) {
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

