#include <algorithm>
#include <iostream>

#include "FUF.h"
#include "Utils.h"
#include "Huffman.h"
#include "WaveReader.h"


struct sortPair {
    bool operator () (const TreeNode* left, const TreeNode* right) {
        return left->d.second < right->d.second;
    }
};


vector<Data> countFrequency(int* data, int size) {
    map<int, int> myMap;
    vector<Data>  frequencies;

    for (int i = 0; i < size; i++) {
        if (myMap.count(data[i])) {
            myMap[data[i]]++;
        }
        else {
            myMap[data[i]] = 1;
        }
	}

    map<int, int>::iterator it;
    for (it = myMap.begin(); it != myMap.end(); it++) {
        frequencies.push_back(Data(it->first, it->second));
    }

    return frequencies;
}

void sortTreeNodes(vector<TreeNode*>& t) {
    sort(t.begin(), t.end(), sortPair());
}

void reduce(vector<TreeNode*>& t) {
    Data d;
    // Add up the frequencies
    d.second = t[0]->d.second + t[1]->d.second;

    // Create the new node, with the branches
    TreeNode* newNode = new TreeNode(d);
    newNode->left = t[0];
    newNode->right = t[1];

    // Remove and add nodes from the list
    t.erase(t.begin(), t.begin()+2);
    t.push_back(newNode);

    sortTreeNodes(t);
}

void Huffman::buildCodeTable(TreeNode* t, string code) {
    if (t == NULL) return;

    if (t->left == NULL && t->right == NULL) {
        codeTable[t->d.first] = code;
    }
    buildCodeTable(t->left,  code+"1");
    buildCodeTable(t->right, code+"0");
}

string Huffman::getCodeFor(Element e) {
    if (codeTable.size() == 0) {
        buildCodeTable(tree);
    }

    return codeTable[e];
}

void Huffman::buildTree(vector<Data> frequencies) {
    this->frequencies = frequencies;
    vector<TreeNode*> treeNodes;

    for (unsigned int i = 0; i < frequencies.size(); i++) {
        treeNodes.push_back(new TreeNode(frequencies[i]));
    }

	//Inserir a árvore dentro do arquivo
    // Ordenados!
    sortTreeNodes(treeNodes);

    // Now, reduce!
    while (treeNodes.size() > 1) {
        reduce(treeNodes);
    }
    tree = treeNodes[0];
}

void Huffman::huffmanize(int* samples, int size) {
    lastByteBitCount = 0;
    data.clear();

    Byte* current = NULL;
    lastByteBitCount = 8;

    for (int i = 0; i < size; i++) {
        string code = getCodeFor(samples[i]);
        for (unsigned int c = 0; c < code.size(); c++) {
            if (lastByteBitCount == 8) {
                if (current != NULL) {
                    data.push_back(*current);
                }
                lastByteBitCount = 0;
                current = new Byte(0x00);
            }
            if (code[c] == '0') {
                *current <<= 1;
                *current &= 0xfe;
            }
            else /* if (code[c] == '1') */ {
                *current <<= 1;
                *current |= 0x01;
            }
            lastByteBitCount++;
        }
    }
    *current <<= 8-lastByteBitCount;
    data.push_back(*current);
}

vector<int> Huffman::dehuffmanize() {
    vector<int> result;

    TreeNode* t = tree;
    int size = data.size() - 1;

    for (unsigned int i = 0; i <= size; i++) {
        Byte current = data[i];

        for (unsigned int j = 0; (i < size && j < 8) || j < lastByteBitCount; j++) {
            Byte byte = (0x80 & current) >> 7;
            current <<= 1;

            if (byte == 0) {
                // Move right
                t = t->right;
            }
            else /* byte == 1 */ {
                // Move left
                t = t->left;
            }

            if (t->left == NULL && t->right == NULL) {
                // This is a leaf!
                result.push_back(t->d.first);

                // Reset the things
                t = tree;
            }
        }
    }

    return result;
}

HuffmanHeader Huffman::getHeader() {
    HuffmanHeader header;
    header.byteCount = data.size();
    header.lastByteBitCount = lastByteBitCount;
    header.sampleCount = frequencies.size();

    return header;
}

int* Huffman::getFrequencies() {
    int size = frequencies.size()*2;
    int* result = new int[size];
    for (int i = 0, j = 0; j < size; i++) {
        result[j] = frequencies[i].first;
        j++;
        result[j] = frequencies[i].second;
        j++;
    }
    return result;
}

Byte* Huffman::getData() {
    int size = data.size();
    Byte* result = new Byte[size];
    for (int i = 0; i < size; i++) {
        result[i] = data[i];
    }
    return result;
}

void Huffman::readFile(FILE* file) {
    HuffmanHeader header;
    fread(&header, sizeof(HuffmanHeader), 1, file);

    int size = header.sampleCount*2;
    int* freq = new int[size];
    fread(freq, sizeof(int), size, file);

    for (int i = 0; i < size;) {
        Data d;
        d.first = freq[i];
        i++;
        d.second = freq[i];
        i++;
        frequencies.push_back(d);
    }
    delete[] freq;

    size = header.byteCount;
    Byte* bytes = new Byte[size];
    fread(bytes, sizeof(Byte), size, file);
    for (int i = 0; i < size; i++) {
        data.push_back(bytes[i]);
    }
    delete[] bytes;

    buildTree(frequencies);

    lastByteBitCount = header.lastByteBitCount;
}

void FUF::huffmanCompress() {
	// Node vector with frequencies
	// Count the frequencies
	int* samples = compressedData->getDataFromAllChannels();
	int size = compressedData->getDataLength()*compressedData->getChannelCount();
    vector<Data> frequencies = countFrequency(samples, size);

    // Mapped!
//    printFrequency(frequencies);

    // Create the tree
    compressedData->huff.buildTree(frequencies);
    // Tree is built!
//    printTree(tree[0], 1);

    compressedData->huff.huffmanize(samples, size);

	compressedData->hasHuffman = true;
}

void FUF::huffmanDecompress() {
    vector<int> result = compressedData->huff.dehuffmanize();

    int cn = compressedData->getChannelCount();
    int dn = compressedData->getDataLength();

	for (int c = 0, i = 0; c < cn; c++) {
	    int* receive = new int[dn];
		for (int d = 0; d < dn; d++) {
			receive[d] = result[i++];
		}
		compressedData->setData(c, receive, dn);
	}
}

