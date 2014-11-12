#include "FUF.h"

FUF::FUF(const char* filename, const char* filenameExtensionLess) : sample(filename) {
	sample.readWav();
}

FUF::~FUF(){ }