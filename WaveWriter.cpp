#include "WaveWriter.h"

void writeInt32(int data) {

}

void writeInt16(int data) {

}

void writeInt8(int data) {
  fwrite(buffer, 1, 1, mFile);

  
}

void WaveWriter::write(int bytes, int* data) {
	switch (bytes) {
		case 1:
			writeInt8(data);
		case 2:
			writeInt16(data);
		case 4:
			writeInt32(data);
	}
}

void WaveWriter::writeWav(const char* filename) {
  mFile = fopen(filename, "wb");

  ...
}
