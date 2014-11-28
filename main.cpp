#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <set>
#include <map>

#include "FUF.h"
#include "Utils.h"


#define USAGE "\
Usage: " PROGRAM_NAME " OPTION FILE\n\
Try \"" PROGRAM_NAME " --help\" for more informations.\n"

#define HELP "\
Usage: " PROGRAM_NAME " OPTION FILE\n\
Compress or decompress a file to .fuf according to the specified algorithm.\n\
OPTION specify decompression or the type of compression algorithm.\n\
\n\
Compress options:\n\
  -h    Huffman compression\n\
  -dh   Differencial, Huffman compression\n\
  -dt   Differencial, Transform compression\n\
  -dth  Differencial, Transform, Huffman compression\n\
  -t    Transform compression\n\
  -td   Transform, Differencial compression\n\
  -tdh  Transform, Differencial, Huffman compression\n\
\n\
Decompress option:\n\
  --dec     Decompress a .fuf file\n\
\n\
\n\
The Transform compression used is the MDCT (Modified Discrete Cossine Transform) algorithm.\n\
\n\
Fun fact: .fuf stands for 'Formato Ultra Foda'.\n\n"

#define BAD_OPTION   "Bad option: "
#define NO_SUCH_FILE "No such file: "

using namespace std;


bool processInput(int argc, char** argv, string& option, string &filename);


int main(int argc, char** argv) {
    string option   = "";
    string filename = "";

    if (!processInput(argc, argv, option, filename)) {
        cout << USAGE << endl;
        return EXIT_SUCCESS;
    }

    // Remove the extension
    string noExtentionFilename = filename.substr(0, filename.find_last_of("."));
    cout << noExtentionFilename << endl;


	// Object Instance
	FUF fuf;

    if (option == "--debug") {
        // Debug option!

        // TODO colocar código de depuração aqui!
        fuf.readFromFile(filename.c_str(), EXTENSION_WAV);

        printWavInfo(fuf.sample->info);

//        fuf.compress(TRANSFORM);
//        fuf.decompress();
//        for (int c = 0; c < fuf.sample->data.channelCount; c++) {
//            fuf.sample->data.data[c] = fuf.compressedData->getData(c);
//        }

//        fuf.writeToFile(noExtentionFilename.c_str(), EXTENSION_WAV);

        return EXIT_SUCCESS;
    }

	if (option == "--dec") {
	    fuf.readFromFile(filename.c_str(), EXTENSION_FUF);

		cout << "Decompressing file " << filename << endl;
        fuf.decompress();

		fuf.writeToFile(noExtentionFilename.c_str(), EXTENSION_WAV);
    }
    else {
        fuf.readFromFile(filename.c_str(), EXTENSION_WAV);

        compressMode algorithm[] = { NONE, NONE, NONE };
        map<char, compressMode> compressMap;
        compressMap['h'] = HUFFMAN;
        compressMap['t'] = TRANSFORM;
        compressMap['d'] = DIFFERENCE;

        switch(option.size()) {
            case 4:
                algorithm[2] = compressMap[option[3]];
            case 3:
                algorithm[1] = compressMap[option[2]];
            case 2:
                algorithm[0] = compressMap[option[1]];
                break;
        }

        fuf.compress(algorithm[0], algorithm[1], algorithm[2]);
        cout << "File compressed successfully!" << endl;
		fuf.writeToFile(noExtentionFilename.c_str(), EXTENSION_FUF);
	}

    return EXIT_SUCCESS;
}


bool processInput(int argc, char** argv, string& option, string &filename) {
    string s1, s2 = "";
    if (argc > 1) {
        s1 = string(argv[1]);
    }
    if (argc > 2) {
        s2 = string(argv[2]);
    }

    if (s1[0] == '-') {
        option = s1;
        filename = s2;
    }
    else if (s2[0] == '-') {
        option = s2;
        filename = s1;
    }

    if (argc < 2 || argc > 3) {
        return false;
    }

    set<string> options;
    options.insert("-h");
    options.insert("-dh");
    options.insert("-dt");
    options.insert("-dth");
    options.insert("-t");
    options.insert("-td");
    options.insert("-tdh");
    options.insert("--dec");

    options.insert("--deb");

    bool isHelp = (option == "--help");
    bool badOption = (argc > 2 && !options.count(option));
    bool invalidFileName = (argc > 2 && access(filename.c_str(), F_OK) == -1);

    if (isHelp) {
        cout << HELP << endl;
        exit(EXIT_SUCCESS);
    }

    if (badOption) {
        cout << BAD_OPTION << option << endl;
        return false;
    }
    if (invalidFileName) {
        cout << NO_SUCH_FILE << filename << endl;
        return false;
    }

    return true;
}

