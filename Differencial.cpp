#include "FUF.h"

#include <stdio.h>
#include "Utils.h"

using namespace std;

int** diffVectorGenerate(int** data, int cn, int dn);

void FUF::differencialCompress() {

	WaveData* data = &(sample.data);
	int cn = data->channelCount;
	int dn = data->dataLength;

	for (int c = 0; c < cn; c++) {
		for (int d = 0; d < dn; d++){
			cout << data->data[c][d] << " ";
		}
		cout << "\n\n";
	}

	int** diff = diffVectorGenerate(data->data, cn, dn);

	for (int c = 0; c < cn; c++) {
		for (int d = 0; d < dn; d++){
			cout << diff[c][d] << " ";
		}
		cout << "\n\n";
	}
}

void FUF::differencialDecompress() {}

int** diffVectorGenerate(int** data, int cn, int dn){
	int** diff = new int*[cn];

	for (int c = 0; c < cn; c++) {
		diff[c] = new int[dn];
		for (int d = 0; d < dn; d++){
			data[c][d] = 0;
		}
	}

	return diff;
}