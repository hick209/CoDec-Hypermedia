#include "FormatoUltraFodaData.h"


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
    FILE* file = fopen(filename, "rb");
    // Read the FUF header
    fread(&fHeader, sizeof(FUFHeader), 1, file);
    // Read the huffman
//    fread(huffHeader, sizeof(HuffmanHeader), fHeader.huffmanHeaderSize, file); // TODO

    // Prepare some content
    channelCount = fHeader.info.channels;

    for (int i = 0; i < fHeader.compressCount; i++) {
        if (fHeader.modes[i] == TRANSFORM) {
            hasTransform = true;
        }
        else if (fHeader.modes[i] == HUFFMAN) {
            hasHuffman = true;
        }
    }

    // Read the data
    if (hasHuffman) {
        // TODO
        int huffSize = 7;
        char* buffer = new char[huffSize]; // TODO
        fread(buffer, sizeof(char), huffSize, file); // TODO
    }
    else {
        // Read the data
        serialData = new int[fHeader.dataSize];
        fread(serialData, sizeof(int), fHeader.dataSize, file);

        // Adapt the data
        dataLength = fHeader.dataSize / channelCount;
        serialData = new int[channelCount*dataLength];
        data = new int*[channelCount];
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

    fHeader.huffmanHeaderSize = 0; // TODO change this
    fHeader.dataSize = hasHuffman ? /* TODO Huff size */ 0 : dataLength;

    fwrite(&fHeader, sizeof(FUFHeader), 1, file);

    // TODO write huffman header
}

void FormatoUltraFodaData::writeData(FILE* file) {
    // write the data
    if (hasHuffman) {
        char buffer[] = "Hello!"; // TODO
        fwrite(buffer, sizeof(char), 7, file); // TODO
    }
    else {
        int* buffer = getDataFromAllChannels();
        fwrite(buffer, sizeof(int), dataLength*channelCount, file);
    }
}

