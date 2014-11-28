#ifndef FUF_DATA_H
#define FUF_DATA_H

#define COMPRESSION_MODE_COUNT 3

#include <stdio.h>
#include "Wave.h"


enum compressMode {
	NONE       = 0,
	HUFFMAN    = 1,
	DIFFERENCE = 2,
	TRANSFORM  = 3
};

struct FUFHeader {
    WaveInfo     info;
    compressMode modes[COMPRESSION_MODE_COUNT];
    int          bytesPerSample;
    int          compressCount;
    int          channelCount;
    long         huffmanHeaderSize;
    long         dataSize;
};


class FormatoUltraFodaData {
private:
    int channelCount;
    int dataLength;

    int** data;
    int*  serialData;

    // TODO add Huffman stuff

public:
	FormatoUltraFodaData(WaveData*);
	FormatoUltraFodaData(const char* filename);
	~FormatoUltraFodaData() {}

    int  getChannelCount();
    int* getData(int channel);
    int  getDataLength();

    int* getDataFromAllChannels();

    void setData(int channel, int* data, int length);

    void writeHeaders(FILE*, WaveInfo, compressMode*, int);
    void writeData(FILE*);

    FUFHeader fHeader;
	double**  zData;
	char*     hData;

	bool hasHuffman;
	bool hasTransform;
};


#endif // FUF_DATA_H

