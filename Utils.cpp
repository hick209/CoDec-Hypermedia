#include "Utils.h"

#include <iostream>
#include <stdio.h>


using namespace std;


void printWavInfo(WaveInfo info) {
	cout << " Wave File Info " << endl;
	cout << "================" << endl;
	cout << "Format Size   : " << info.formatSize << endl;
	cout << "Format        : " << info.format << endl;
	cout << "Channel Count : " << info.channelCount << endl;
	cout << "Sample Rate   : " << info.sampleRate << endl;
	cout << "BPS           : " << info.bitsPerSecond << endl;
	cout << "Bit Depth     : " << info.bitDepth << endl;
	cout << "Data Size     : " << info.dataSize << endl;
}
