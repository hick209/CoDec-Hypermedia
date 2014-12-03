#include "FUF.h"

#include <stdio.h>
#include "Utils.h"

using namespace std;


void FUF::differencialCompress() {
    int cn = compressedData->getChannelCount();
    int dn = compressedData->getDataLength();

	for (int c = 0; c < cn; c++) {
	    int* data = compressedData->getData(c);
	    int* diff = new int[dn];

		diff[0] = data[0];
		for (int d = 1; d < dn; d++) {
			diff[d] = data[d] - data[d - 1];
		}

		compressedData->setData(c, diff, dn);
	}
}

void FUF::differencialDecompress() {
    int cn = compressedData->getChannelCount();
    int dn = compressedData->getDataLength();
	
	for (int c = 0; c < cn; c++) {
		int* diff = compressedData->getData(c);
		int* recv = new int[dn];

		recv[0] = diff[0];
		for (int d = 1; d < dn; d++) {
			recv[d] = diff[d] + recv[d - 1];
		}
		
		compressedData->setData(c, recv, dn);
		
	}
}