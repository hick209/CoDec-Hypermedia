#ifndef FUF_H
#define FUF_H

#define COMPRESSION_MODE_COUNT 3

#include <stdio.h>
#include "WaveReader.h"

class FUF {
private:
	WaveReader sample;
	
	enum {
		NONE = 0,
		HUFFMAN = 1,
		DIFFERENCE = 2,
		TRANSFORM = 3
	} compression[COMPRESSION_MODE_COUNT];
	int lastCompression;

public:

	FUF(const char* filename, const char* filenameExtensionLess);
	~FUF();

	void compress(compressMode);
	void compress(compressMode, compressMode);
	void compress(compressMode, compressMode, compressMode);
	void huffmanCompress();
	void differencialCompress();
	void transformCompress();
	void decompression();

};


#endif // FUF_H