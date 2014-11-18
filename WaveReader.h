#ifndef WAVE_READER_H
#define WAVE_READER_H

#include <stdio.h>


struct WaveInfo {
	/* Format */
	int formatSize;
	int format;
	int channels;
	int channelCount;
	int sampleRate;
	int bitsPerSecond;
	int formatBlockAlign;
	int bitDepth;
	/* Riff */
	int headerId;
	int memSize;
	int riffStyle;
	/* Data */
	int dataSize;
};


struct WaveData {
	int   channelCount;
	int** data;
	int   dataLength;
};


class WaveReader {
private:
	enum WavChunks {
	    RIFF_HEADER     = 0x46464952,
	    WAV_RIFF        = 0x54651475,
	    FORMAT          = 0x20746d66,
	    LABELED_TEXT    = 0x478747C6,
	    INSTRUMENTATION = 0x478747C6,
	    SAMPLE          = 0x6C706D73,
	    FACT            = 0x47361666,
	    DATA            = 0x61746164,
	    JUNK            = 0x4b4e554a
	};

	enum WavFormat {
	    PULSE_CODE_MODULATION = 0x01,
	    IEEE_FLOATING_POINT   = 0x03,
	    A_LAW                 = 0x06,
	    MU_LAW                = 0x07,
	    IMAADPCM              = 0x11,
	    YAMAHA_ITUG723ADPCM   = 0x16,
	    GSM610                = 0x31,
	    ITUG721ADPCM          = 0x40,
	    MPEG                  = 0x50,
	    EXTENSIBLE            = 0xFFFE
	};

	FILE* mWavFile;

	int readInt32();
	int readInt16();
	int readInt8();

	int readSample(int bytes);

	WaveInfo readHeader();
	WaveData readData();

public:
	WaveInfo info;
	WaveData data;

	WaveReader(const char* filename);
	~WaveReader();

	void readWav();
};


#endif // WAVE_READER_H

