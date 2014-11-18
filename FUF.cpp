#include "FUF.h"
#include <algorithm>

FUF::FUF(const char* filename, const char* filenameExtensionLess)
		: sample(filename), lastCompression(0){
	for (int i = 0, i < COMPRESSION_MODE_COUNT; i++){
		compress[i] = NONE;
	}
	sample.readWav();
}

FUF::~FUF(){}

FUF::compress(compressMode a){
	for (int i = 0, i < lastCompression; i++){
		if (compress[i] == a) {
			cout << "This compression was already applied. YOCO, bro!";
			return;
		}
	}
	compress[lastCompression] = a;
	lastCompression++;
}

FUF::compress(compressMode a, compressMode b){
	compress(a);
	compress(b);
}

FUF::compress(compressMode a, compressMode b, compressMode c){
	compress(a);
	compress(b);
	compress(c);
}