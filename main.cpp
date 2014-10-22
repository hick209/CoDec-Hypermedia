#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include "Huffman.h"
#include "Differencial.h"
#include "Transform.h"
#include "Decompression.h"

#define USAGE "\
Usage: " PROGRAM_NAME " OPTION FILE\n\
Try \"" PROGRAM_NAME " --help\" for more informations.\n"

#define HELP "\
Usage: " PROGRAM_NAME " OPTION FILE\n\
Compress or decompress a file to .fuf according to the specified algorithm.\n\
OPTION specify decompression or the type of compression algorithm.\n\
\n\
Compress options:\n\
  --huff    Huffman compression\n\
  --diff    Differencial compression\n\
  --trans   Transform compression\n\
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


    if (option == "--huff") {
        cout << "Applying Huffman to file " << filename << endl;
        huffmanCompress(filename.c_str(), noExtentionFilename.c_str());
    }
    else if (option == "--diff") {
        cout << "Applying Differencial to file " << filename << endl;
        differencialCompress(filename.c_str(), noExtentionFilename.c_str());
    }
    else if (option == "--trans") {
        cout << "Applying Transform to file " << filename << endl;
        transformCompress(filename.c_str(), noExtentionFilename.c_str());
    }
    else if (option == "--dec") {
        cout << "Decompressing file " << filename << endl;
        decompression(filename.c_str(), noExtentionFilename.c_str());
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
    bool badOption = (argc > 2 && option != "--huff" && option != "--trans" && option != "--diff" && option != "--dec");
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

