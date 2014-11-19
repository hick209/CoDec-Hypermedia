#include "FUF.h"

using namespace std;

FUF::FUF(const char* filename)
		: sample(filename), lastCompression(0){
	for (int i = 0; i < COMPRESSION_MODE_COUNT; i++) {
		compression[i] = NONE;
	}
	sample.readWav();
}

FUF::~FUF() {}

void FUF::writeToFile(const char* filename){
	WaveWriter output(sample.info, sample.data);
	output.writeWav(filename);
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
			huffmanCompress();
			break;
		case DIFFERENCE:
			differencialCompress();
			break;
		case TRANSFORM:
			transformCompress();
			break;
		case NONE:
			break;
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
				huffmanDecompress();
				break;
			case DIFFERENCE:
				differencialDecompress();
					break;
			case TRANSFORM:
				transformDecompress();
				break;
			case NONE:
				break;
		}
	}
}

