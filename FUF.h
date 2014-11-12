#ifndef FUF_H
#define FUF_H

#include <stdio.h>
#include "WaveReader.h"


class FUF {
private:
	WaveReader sample;
	int huffmanized;
	int differentiated;
	int transformed;

public:

	FUF(const char* filename, const char* filenameExtensionLess);
	~FUF();

	void huffmanCompress();
	void differencialCompress();
	void transformCompress();
	void decompression();

};


#endif // FUF_H