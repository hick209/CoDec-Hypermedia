#include "FUF.h"

#include <stdio.h>
#include "Utils.h"

using namespace std;

int** diffVectorGenerate(int** data, int cn, int dn);

void FUF::differencialCompress() {

	WaveData* wdata = &(sample.data);
	int cn = wdata->channelCount;
	int dn = wdata->dataLength;
	int** data = wdata->data;

	int** diff = new int*[cn];

	for (int c = 0; c < cn; c++) {
		diff[c] = new int[dn];
		diff[c][0] = data[c][0];
		for (int d = 1; d < dn; d++){
			diff[c][d] = data[c][d] - data[c][d - 1];
		}
	}

	data = diff;
}

void FUF::differencialDecompress() {
	WaveData* wdata = &(sample.data);
	int cn = wdata->channelCount;
	int dn = wdata->dataLength;
	int** diff = wdata->data;

	int** recv = new int*[cn];

	for (int c = 0; c < cn; c++) {
		recv[c] = new int[dn];
		recv[c][0] = diff[c][0];
		for (int d = 1; d < dn; d++){
			recv[c][d] = diff[c][d] + diff[c][d - 1];
		}
	}

	diff = recv;
}