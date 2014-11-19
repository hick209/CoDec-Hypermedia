#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include "FUF.h"

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
  -d    Differencial, Huffman compression\n\
  -t    Transform, Huffman compression\n\
  -dt   Differencial, Transform, Huffman compression\n\
  -td   Transform, Differencial, Huffman compression\n\
\n\
Decompress option:\n\
  --dec     Decompress a .fuf file\n\
\n\
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
	if (option == "--dec") {
		FUF sample(filename.c_str(), true);
        cout << "Decompressing file " << filename << endl;
        sample.decompress();
		sample.writeToFile((noExtentionFilename + ".wav").c_str());
    }
    else {
		FUF sample(filename.c_str());
        if (option[1] == 'h') {
            cout << "Applying Huffman to file " << filename << endl;
            sample.compress(HUFFMAN);
        }
        else {
            switch(option.size()) {
            case 2:
                if (option[1] == 'd') {
                    cout << "Applying Differencial, Huffman to file " << filename << endl;
                    sample.compress(DIFFERENCE, HUFFMAN);
                }
                else if (option[1] == 't') {
                    cout << "Applying Transform, Huffman to file " << filename << endl;
                    sample.compress(TRANSFORM, HUFFMAN);
                }
                break;

            case 3:
                if (option[1] == 'd' && option[2] == 't') {
                    cout << "Applying Differencial, Transform, Huffman to file " << filename << endl;
                    sample.compress(DIFFERENCE, TRANSFORM, HUFFMAN);
                }
                else if (option[1] == 't' && option[2] == 'd') {
                    cout << "Applying Differencial, Transform, Huffman to file " << filename << endl;
                    sample.compress(TRANSFORM, DIFFERENCE, HUFFMAN);
                }
            }
        }

		sample.writeFufFile((noExtentionFilename + ".fuf").c_str());
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

    bool isHelp = (option == "-h" || option == "--help");
    bool badOption = (argc > 2 && option != "-h" && option != "-t" && option != "-d" && option != "-td" && option != "-dt" && option != "--dec");
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

