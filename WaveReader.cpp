#include "WaveReader.h"


WaveReader::WaveReader(const char* filename) {
	mWavFile = fopen(filename, "rb");
	info = readHeader();
}

WaveReader::~WaveReader() {
	fclose(mWavFile);
}

int WaveReader::readInt32() {
	int data;
	fread(&data, 4, 1, mWavFile);

	return data;
}

int WaveReader::readInt16() {
	int data;
	fread(&data, 2, 1, mWavFile);

	return data;
}


WaveInfo WaveReader::readHeader() {
	int  chunkId   = 0;
	bool dataChunk = false;

	WaveInfo info;

	while (!dataChunk) {
	    chunkId = readInt32();
	    switch (chunkId) {
		    case FORMAT:
		        info.formatSize = readInt32();
		        info.format = readInt16();
		        info.channels = readInt16();
		        info.channelCount = (int) info.channels;
		        info.sampleRate = readInt32();
		        info.bitsPerSecond = readInt32();
		        info.formatBlockAlign = readInt16();
		        info.bitDepth = readInt16();

		        if (info.formatSize == 18) {
		            int extraData = readInt16();
		            fseek(mWavFile, extraData, SEEK_CUR);
		        }
		        break;

		    case RIFF_HEADER:
		        info.headerId = chunkId;
		        info.memSize = readInt32();
		        info.riffStyle = readInt32();
		        break;

		    case DATA:
		        dataChunk = true;
		        info.dataSize = readInt32();
		        break;

		    default:
		        int skipSize = readInt32();
		        fseek(mWavFile, skipSize, SEEK_CUR);
		        break;
	    }
	}

	return info;
}
