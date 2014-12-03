#ifndef FUF_H
#define FUF_H

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "FormatoUltraFodaData.h"
#include "WaveReader.h"
#include "WaveWriter.h"


enum fileExtension {
	EXTENSION_WAV,
	EXTENSION_FUF
};

class FUF {
public:
	WaveReader*           sample;
	FormatoUltraFodaData* compressedData;

	compressMode compression[COMPRESSION_MODE_COUNT];
	int          lastCompression;

	void huffmanCompress();
	void differencialCompress();
	void transformCompress();

	void huffmanDecompress();
	void differencialDecompress();
	void transformDecompress();

public:
	FUF():sample(NULL),compressedData(NULL){}
	~FUF();

	void compress(compressMode);
	void compress(compressMode, compressMode);
	void compress(compressMode, compressMode, compressMode);

	void decompress();

    void readFromFile(const char* filename, fileExtension);
	void writeToFile(const char* filename, fileExtension);
};

#endif // FUF_H
