#ifndef WAVE_WRITER_H
#define WAVE_WRITER_H

#include <stdio.h>
#include "WaveReader.h"


class WaveWriter {
private:
	const WaveInfo info;
	const WaveData data;

  FILE* mFile;

	void writeInt32(int data);
	void writeInt16(int data);
	void writeInt8(int data);

	void write(int bytes, int* data);

	void writeHeader();
	void writeData();

public:
	WaveWriter(WaveInfo, WaveData);
	~WaveWriter();

	void writeWav(const char* filename);
};


#endif // WAVE_WRITER_H

