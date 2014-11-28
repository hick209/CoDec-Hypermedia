#include "WaveReader.h"


WaveReader::WaveReader(const char* filename) {
	mWavFile = fopen(filename, "rb");
}

WaveReader::~WaveReader() {
	fclose(mWavFile);
}

void WaveReader::readWav() {
	printf("Reading header...\n");
	info = readHeader();
	printf("Reading data...\n");
	data = readData();
}


int WaveReader::readInt32() {
	int data = 0;
	char buffer[4];
	fread(buffer, 4, 1, mWavFile);

	data += 0x00ff & buffer[3];
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

	data += 0x00ff & buffer[1];
	data = data << 8;
	data += 0x00ff & buffer[0];

	return data;
}

int WaveReader::readInt8() {
	int data = 0;
	char buffer[1];
	fread(buffer, 1, 1, mWavFile);

	data = 0x00ff & buffer[0];

	return data;
}

int WaveReader::readSample(int bytes) {
	switch (bytes) {
		case 1:
			return readInt8();
		case 2:
			return readInt16();
		case 4:
			return readInt32();
	}

	return 0;
}


WaveInfo WaveReader::readHeader() {
	int  chunkId   = 0;
	bool dataChunk = false;

	WaveInfo info;

	while (!dataChunk) {
	    chunkId = readInt32();
	    switch (chunkId) {
		    case FORMAT:
		    	//printf("Format\n");
		        info.formatSize = readInt32();
		        info.format = readInt16();
		        info.channels = readInt16();
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
		    	//printf("Riff Header\n");
		        info.headerId = chunkId;
		        info.memSize = readInt32();
		        info.riffStyle = readInt32();
		        break;

		    case DATA:
		    	//printf("Data\n");
		        dataChunk = true;
		        info.dataSize = readInt32();
		        break;

		    default:
		        int skipSize = readInt32();
		        //printf("Skip Size = %d\n", skipSize);
		        fseek(mWavFile, skipSize, SEEK_CUR);
		        break;
	    }
	}

	return info;
}


WaveData WaveReader::readData() {
	WaveData data;
	data.channelCount = info.channels;

	data.dataLength = info.dataSize / data.channelCount;
	data.data = new int*[data.channelCount];
	for (int i = 0; i < data.channelCount; i++) {
		data.data[i] = new int[data.dataLength];
	}

	int bytesPerSample;

	switch (info.format) {
		case PULSE_CODE_MODULATION:
		case EXTENSIBLE:
		default:
			bytesPerSample = (info.bitDepth / data.channelCount) / 8;
			break;
	}

	int byteCount = info.dataSize / data.channelCount;
	for (int i = 0; i < byteCount; i++) {
		for (int j = 0; j < data.channelCount; j++) {
			data.data[j][i] = readSample(bytesPerSample);
		}
	}

	return data;
}

