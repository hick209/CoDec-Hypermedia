#include "WaveReader.h"


WaveReader::WaveReader(const char* filename) {
	mWavFile = fopen(filename, "rb");
}

WaveReader::~WaveReader() {
	fclose(mWavFile);
}

int WaveReader::readInt32() {
	int data = 0;
	char buffer[4];
	fread(buffer, 4, 1, mWavFile);

	data = 0x00ff & buffer[3];
	data = data << 8;
	data += 0x00ff & buffer[2];
	data = data << 8;
	data += 0x00ff & buffer[1];
	data = data << 8;
	data += 0x00ff & buffer[0];

	return data;
}

int WaveReader::readInt16() {
	int data = 0;
	char buffer[2];
	fread(buffer, 2, 1, mWavFile);

	data = 0x00ff & buffer[1];
	data = data << 8;
	data += 0x00ff & buffer[0];

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


WaveData WaveReader::getData() {
	WaveData data;
	data.channelCount = info.channelCount;

	data.dataLength = new int[data.channelCount];
	data.data = new char*[data.channelCount];

	// TODO

	return data;
}


void WaveReader::readWav() {
	info = readHeader();
	data = readData();
}
