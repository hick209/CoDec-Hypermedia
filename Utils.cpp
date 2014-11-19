#include "Utils.h"

#include <iostream>
#include <stdio.h>


using namespace std;


void printWavInfo(WaveInfo info) {
	cout << " Wave File Info " << endl;
	cout << "================" << endl;
	cout << "Format        : " << info.format           << endl;
	cout << "Format Size   : " << info.formatSize       << endl;
	cout << "Channel Count : " << info.channels         << endl;
	cout << "Sample Rate   : " << info.sampleRate       << endl;
	cout << "BPS           : " << info.bitsPerSecond    << endl;
	cout << "Bit Depth     : " << info.bitDepth         << endl;
	cout << "Data Block    : " << info.formatBlockAlign << endl;
	cout << "Data Size     : " << info.dataSize         << endl;
}


void printWavData(WaveData d) {
	cout << " Wave Data " << endl;
	cout << "===========" << endl;

	const int SAMPLES = 20;
	for (int j = 0; j < d.channelCount; j++) {
		cout << "Channel " << j << ":";
		for (int i = 0; i < SAMPLES; i++) {
			printf(" %4d", d.data[j][i]);
		}
		cout << endl;
	}
}

