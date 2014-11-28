#include "WaveWriter.h"

WaveWriter::WaveWriter(WaveInfo info, WaveData data)
    : mInfo(info), mData(data) {
}

void WaveWriter::writeWav(const char* filename) {
	mFile = fopen(filename, "wb");

	printf("Writing header...\n");
	writeHeader();
	printf("Writing data...\n");
	writeData();

	fclose(mFile);
}

void WaveWriter::writeInt32(int data) {
    unsigned char buffer[4];

	buffer[0] = (0x000000ff & data) >>  0;
	buffer[1] = (0x0000ff00 & data) >>  8;
	buffer[2] = (0x00ff0000 & data) >> 16;
	buffer[3] = (0xff000000 & data) >> 24;

    fwrite(buffer, 4, 1, mFile);
}

void WaveWriter::writeInt16(int data) {
    unsigned char buffer[2];

	buffer[0] = (0x000000ff & data) >> 0;
	buffer[1] = (0x0000ff00 & data) >> 8;

    fwrite(buffer, 2, 1, mFile);
}

void WaveWriter::writeInt8(int data) {
    unsigned char buffer[1];

    buffer[0] = (0x000000ff & data);

    fwrite(buffer, 1, 1, mFile);
}

void WaveWriter::write(int bytes, int data) {
	switch (bytes) {
		case 1:
			writeInt8(data);
			break;
		case 2:
			writeInt16(data);
			break;
		case 4:
			writeInt32(data);
			break;
	}
}

void WaveWriter::writeHeader() {  
    // RIFF_HEADER
    writeInt32(mInfo.headerId);
    writeInt32(mInfo.memSize);
    writeInt32(mInfo.riffStyle);

    // FORMAT
    writeInt32(FORMAT);
    writeInt32(mInfo.formatSize);
    writeInt16(mInfo.format);
    writeInt16(mInfo.channels);
    writeInt32(mInfo.sampleRate);
    writeInt32(mInfo.bitsPerSecond);
    writeInt16(mInfo.formatBlockAlign);
    writeInt16(mInfo.bitDepth);
    if (mInfo.formatSize == 18) {
        writeInt16(0);
    }

    // DATA
    writeInt32(DATA);
    writeInt32(mInfo.dataSize);
}

void WaveWriter::writeData() {
    int size = mData.dataLength;
    int channels = mData.channelCount;

	int bytesPerSample;

	switch (mInfo.format) {
		case PULSE_CODE_MODULATION:
		case EXTENSIBLE:
		default:
			bytesPerSample = (mInfo.bitDepth / channels) / 8;
			break;
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < channels; j++) {
			write(bytesPerSample, mData.data[j][i]);
		}
	}
}

