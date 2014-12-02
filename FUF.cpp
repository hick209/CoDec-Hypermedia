#include "FUF.h"

using namespace std;


FUF::~FUF() {
    if (sample)         delete sample;
    if (compressedData) delete compressedData;
}

void FUF::readFromFile(const char* filename, fileExtension ext) {
    if (ext == EXTENSION_WAV) {
    	sample = new WaveReader(filename);
	    for (int i = 0; i < COMPRESSION_MODE_COUNT; i++) {
		    compression[i] = NONE;
	    }
	    lastCompression = 0;

	    sample->readWav();
	    compressedData = new FormatoUltraFodaData(&(sample->data));
    }
	else if (ext == EXTENSION_FUF) {
	    compressedData = new FormatoUltraFodaData(filename);
	    lastCompression = compressedData->fHeader.compressCount;
	    for (int i = 0; i < lastCompression; i++) {
		    compression[i] = compressedData->fHeader.modes[i];
	    }
	}
}

void FUF::writeToFile(const char* filename, fileExtension ext) {
	if (ext == EXTENSION_WAV) {
        WaveData waveData;
        waveData.channelCount = compressedData->fHeader.channelCount;
        waveData.data = new int*[waveData.channelCount];
        for (int i = 0; i< waveData.channelCount; i++) {
            waveData.data[i] = compressedData->getData(i);
        }
        waveData.dataLength = compressedData->getDataLength();
	    WaveWriter output(compressedData->fHeader.info, waveData);

	    string fname = string(filename) + ".wav";

	    cout << "Writing file '" << fname << "'..." << endl;

        output.writeWav(fname.c_str());
	}
	else if (ext == EXTENSION_FUF) {
	    // Open the file
        string fname = string(filename) + ".fuf";
        FILE* file = fopen("Test", "wb");

        cout << "Writing file '" << fname << "'..." << endl;

        // Write the headers
        compressedData->writeHeaders(file, sample->info, compression, lastCompression);
        // Write the data
        compressedData->writeData(file);

        fclose(file);
	}
}

void FUF::compress(compressMode a) {
	for (int i = 0; i < lastCompression; i++) {
		if (compression[i] == a) {
			cout << "This compression was already applied. YOCO, bro!" << endl;
			return;
		}
	}

	switch (a) {
		case HUFFMAN:
    		cout << "Applying Huffman..." << endl;
			huffmanCompress();
			break;

		case DIFFERENCE:
		    cout << "Applying Difference..." << endl;
			differencialCompress();
			break;

		case TRANSFORM:
		    cout << "Applying Transform..." << endl;
			transformCompress();
			break;

		case NONE:
		    return;
	}

	compression[lastCompression] = a;
	lastCompression++;
}

void FUF::compress(compressMode a, compressMode b){
	compress(a);
	compress(b);
}

void FUF::compress(compressMode a, compressMode b, compressMode c){
	compress(a);
	compress(b);
	compress(c);
}

void FUF::decompress(){
	while (lastCompression > 0) {
		lastCompression--;
		switch (compression[lastCompression]){
			case HUFFMAN:
			    cout << "Decompressing Huffman..." << endl;
				huffmanDecompress();
				break;

			case DIFFERENCE:
				cout << "Decompressing Difference..." << endl;
				differencialDecompress();
					break;

			case TRANSFORM:
			    cout << "Decompressing Transform..." << endl;
				transformDecompress();
				break;

			case NONE:
				break;
		}
	}
}

