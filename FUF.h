#ifndef FUF_H
#define FUF_H

#define COMPRESSION_MODE_COUNT 3

#include <stdio.h>
#include <iostream>
#include "WaveReader.h"

enum compressMode {
	NONE       = 0,
	HUFFMAN    = 1,
	DIFFERENCE = 2,
	TRANSFORM  = 3
};

class FUF {
private:
	WaveReader sample;
	
	compressMode compression[COMPRESSION_MODE_COUNT];
	int lastCompression;

	void huffmanCompress();
	void differencialCompress();
	void transformCompress();

	void huffmanDecompress();
	void differencialDecompress();
	void transformDecompress();

public:

	FUF(const char* filename, const char* filenameExtensionLess);
	~FUF();

	void compress(compressMode);
	void compress(compressMode, compressMode);
	void compress(compressMode, compressMode, compressMode);
	void decompress();
};


#endif // FUF_H
