#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <utility>
#include <vector>
#include <string>
#include <map>


using namespace std;


typedef unsigned char      Byte;
typedef int                Element;
typedef pair<Element, int> Data;

struct TreeNode;
struct HuffmanHeader;


class Huffman {
private:
    vector<Data>         frequencies;
    TreeNode*            tree;
    vector<Byte>         data;
    char                 lastByteBitCount;
    map<Element, string> codeTable;

    string getCodeFor(Element);
    void   buildCodeTable(TreeNode*, string code = "");

public:
    void buildTree(vector<Data> frequencies);
    void readFile(FILE*);

    void        huffmanize(int* samples, int size);
    vector<int> dehuffmanize();

    HuffmanHeader getHeader();
    int*          getFrequencies();
    Byte*         getData();
};


struct HuffmanHeader {
    size_t byteCount;
    char   lastByteBitCount;

    size_t sampleCount;
};


struct TreeNode {
    Data      d;
    TreeNode* left;
    TreeNode* right;

    TreeNode(Data data)
        : d(data) {
        left  = NULL;
        right = NULL;
    }
};


#endif // HUFFMAN_H

