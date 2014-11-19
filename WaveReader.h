#ifndef WAVE_READER_H
#define WAVE_READER_H

#include <stdio.h>
#include "Wave.h"


class WaveReader {
private:
	FILE* mWavFile;

	int readInt32();
	int readInt16();
	int readInt8();

	int readSample(int bytes);

	WaveInfo readHeader();
	WaveData readData();

public:
	WaveInfo info;
	WaveData data;

	WaveReader(const char* filename);
	~WaveReader();

	void readWav();
};


#endif // WAVE_READER_H

