#include "FormatoUltraFodaData.h"
#include <iostream>

using namespace std;

FormatoUltraFodaData::FormatoUltraFodaData(WaveData* wdata) {
    hasHuffman = false;
    hasTransform = false;
    zData = NULL;
    data = NULL;
    serialData = NULL;

    data = wdata->data;
    channelCount = wdata->channelCount;
    dataLength = wdata->dataLength;
}

FormatoUltraFodaData::FormatoUltraFodaData(const char* filename) {
    hasHuffman = false;
    hasTransform = false;
    zData = NULL;
    data = NULL;
    serialData = NULL;

    FILE* file = fopen(filename, "rb");
    // Read the FUF header
    fread(&fHeader, sizeof(FUFHeader), 1, file);

    // Prepare some content
    channelCount = fHeader.info.channels;
    fHeader.bytesPerSample = (fHeader.info.bitDepth / channelCount) / 8;

    for (int i = 0; i < fHeader.compressCount; i++) {
        if (fHeader.modes[i] == TRANSFORM) {
            hasTransform = true;
        }
        else if (fHeader.modes[i] == HUFFMAN) {
            hasHuffman = true;
        }
    }

    // Read the data

    // Adapt the data
    dataLength = fHeader.dataSize / channelCount;

    // Read the data
    data = new int*[channelCount];

    if (hasHuffman) {
        // Read the huffman
        huff.readFile(file);
    }
    else {
        int size = (dataLength * channelCount * fHeader.bytesPerSample);
        unsigned char* buffer = new unsigned char[size];
		fread(buffer, sizeof(char), size, file);

        serialData = new int[fHeader.dataSize];
        for (int i = 0, j = 0; i < fHeader.dataSize && j < size; i++) {
            serialData[i] = 0;
            switch (fHeader.bytesPerSample) {
                case 4:
                	serialData[i] += 0x00ff & buffer[j++];
                    serialData[i] = serialData[i] << 8;
                	serialData[i] += 0x00ff & buffer[j++];
                	serialData[i] = serialData[i] << 8;
                case 2:
                    serialData[i] += 0x00ff & buffer[j++];
                    serialData[i] = serialData[i] << 8;
                case 1:
                    serialData[i] += 0x00ff & buffer[j++];
                    break;
            }
        }

        for (int c = 0, j = 0; c < channelCount; c++) {
            data[c] = new int[dataLength];
            for (int i = 0; i < dataLength; i++, j++) {
                data[c][i] = serialData[j];
            }
        }
    }
    fclose(file);
}

int  FormatoUltraFodaData::getChannelCount() {
    return channelCount;
}

int* FormatoUltraFodaData::getData(int channel) {
    if (hasTransform && zData != NULL) {
        for (int c = 0; c < channelCount; c++) {
            for (int i = 0; i < dataLength; i++) {
                data[c][i] = (int) zData[c][i];
            }
            delete[] zData[c];
        }
        delete[] zData;
        zData = NULL;
    }
    return data[channel];
}

int FormatoUltraFodaData::getDataLength() {
    return dataLength;
}

int* FormatoUltraFodaData::getDataFromAllChannels() {
    if (serialData == NULL) {
        serialData = new int[channelCount*dataLength];
        for (int c = 0, j = 0; c < channelCount; c++) {
            for (int i = 0; i < dataLength; i++, j++) {
                serialData[j] = data[c][i];
            }
        }
    }

    return serialData;
}

void FormatoUltraFodaData::setData(int channel, int* data, int length) {
    this->data[channel] = data;
    dataLength = length;
}

void FormatoUltraFodaData::writeHeaders(FILE* file, WaveInfo info, compressMode* mode, int compressCount) {
    fHeader.info = info;
    for (int i = 0; i < compressCount; i++) {    
        fHeader.modes[i] = mode[i];
    }
    fHeader.compressCount = compressCount;
    fHeader.channelCount = channelCount;
    fHeader.bytesPerSample = (fHeader.info.bitDepth / channelCount) / 8;

    fHeader.dataSize = dataLength * channelCount;

    fwrite(&fHeader, sizeof(FUFHeader), 1, file);

    // Write huffman header
    if (hasHuffman) {
        HuffmanHeader huffHeader = huff.getHeader();
        fwrite(&huffHeader, sizeof(HuffmanHeader), 1, file);
    }
}

void FormatoUltraFodaData::writeData(FILE* file) {
    // write the data
    if (hasHuffman) {
        HuffmanHeader huffHeader = huff.getHeader();

        int* frequencies = huff.getFrequencies();
        fwrite(frequencies, sizeof(int), huffHeader.sampleCount*2, file);
        delete[] frequencies;

        Byte* bytes = huff.getData();
        fwrite(bytes, sizeof(Byte), huffHeader.byteCount, file);
        delete[] bytes;
    }
    else {
        int* dataFull = getDataFromAllChannels();
        int size = (dataLength * channelCount * fHeader.bytesPerSample);
        unsigned char* buffer = new unsigned char[size];

        for (int i = 0; i < size; ) {
            unsigned int d = (unsigned int) dataFull[i];
            switch (fHeader.bytesPerSample) {
                case 4:
                    buffer[i++] = (char) ((0xff000000 & d) >> 24);
                    buffer[i++] = (char) ((0x00ff0000 & d) >> 16);
                case 2:
                    buffer[i++] = (char) ((0x0000ff00 & d) >>  8);
                case 1:
                    buffer[i++] = (char) ((0x000000ff & d) >>  0);
                    break;
            }
        }

        fwrite(buffer, sizeof(char), size, file);
        delete buffer;
    }
}

