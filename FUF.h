#ifndef FUF_H
#define FUF_H

#define COMPRESSION_MODE_COUNT 3

#include <stdio.h>
#include <iostream>
#include <fstream>
#include "WaveReader.h"
#include "WaveWriter.h"

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

	FUF(const char* filename);
	~FUF();

	void compress(compressMode);
	void compress(compressMode, compressMode);
	void compress(compressMode, compressMode, compressMode);
	void decompress();
	void writeToFile(const char* filename);

	friend std::istream& operator>> (std::istream& stream, const FUF& object);
	friend std::ostream& operator<< (std::ostream& stream, const FUF& object);
};

std::istream& operator>> (std::istream& stream, const FUF& object){
	return stream;
}

std::ostream& operator<< (std::ostream& stream, const FUF& object){
	return stream;
}


#endif // FUF_H
