#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
#include <map>

#include "FUF.h"
#include "Utils.h"
#include "WaveReader.h"

using namespace std;

typedef vector<int>        Element;
typedef pair<Element, int> Data;

struct Code {
		vector<int> code;
		Element e;
};

struct HuffmanRoll {
#define MASK_KEY  0x0f
#define MASK_LEAF 0xf0
    HuffmanRoll(char key, bool isLeaf = false, int data = 0)
        : data(data), isLeaf_key((isLeaf ? 0x10 : 0x00) | (key ? 0x01 : 0x00)) {}

    bool isLeaf() {
        return MASK_LEAF & isLeaf_key;
    }
    char getKey() {
        return MASK_KEY & isLeaf_key;
    }

private:
    const int  data;
    const char isLeaf_key;
};


struct TreeNode {
    Data      d;
    TreeNode* left;
    TreeNode* right;

    TreeNode(Data data)
        : d(data) {
        left = 0;
        right = 0;
    }
};

struct Huffman {
    Huffman(TreeNode t) {
        
    }

private:
    Element a;
};


struct sortPair {
    bool operator () (const TreeNode &left, const TreeNode &right) {
        return left.d.second < right.d.second;
    }
};

void generateCodeTable(TreeNode* t, vector<Code>& codeTable, vector<int>& code);

vector<Data> countFrequency(WaveData d) {
    map<int, int> myMap;
    vector<Data>  frequencies;

    for (int c = 0; c < d.channelCount; c++) {
        for (int i = 0; i < d.dataLength; i++) {
            if (myMap.count(d.data[c][i])) {
	            myMap[d.data[c][i]]++;
	        }
	        else {
                myMap[d.data[c][i]] = 1;
            }
        }
	}

    map<int, int>::iterator it;
    for (it = myMap.begin(); it != myMap.end(); it++) {
        Element e;
        e.push_back(it->first);
        frequencies.push_back(Data(e, it->second));
    }

    return frequencies;
}

void sortTreeNodes(vector<TreeNode>& t) {
    sort(t.begin(), t.end(), sortPair());
}

void reduce(vector<TreeNode>& t) {
    Element elements(t[0].d.first);
    // Join the elements
    elements.assign(t[1].d.first.begin(), t[1].d.first.end());

    Data d;
    d.first = elements;
    // Add up the frequencies
    d.second = t[0].d.second + t[1].d.second;

    // Create the new node, with the branches
    TreeNode newNode(d);
    newNode.left = &t[0];
    newNode.right = &t[1];

    // Remove and add nodes from the list
    t.erase(t.begin(), t.begin()+2);
    t.push_back(newNode);

    sortTreeNodes(t);
}

void printFrequency(vector<Data> f) {
    for (unsigned int i = 0; i < f.size(); i++) {
        cout << "Element: " << f[i].first[0] << " -> " << f[i].second << endl;
    }
}

void printTree(TreeNode* t, int depth) {
    if (t == 0) return;

    printTree(t->left, depth+1);
    cout << depth << " 1ºElement: " << t->d.first[0] << " -> " << t->d.second << endl;
    printTree(t->right, depth+1);
}

void getLeft(TreeNode* t, vector<Code>& codeTable, vector<int>& code){
		Code aux;
		
		if(t->left == NULL){
			aux.code.assign(code.begin(), code.end());
			aux.e.assign(t->d.first.begin(), t->d.first.end());
			code.clear();
			codeTable.push_back(aux);
		}
		else{
			code.push_back(0);
			generateCodeTable(t->left, codeTable, code);
		}
}

void getRight(TreeNode* t, vector<Code>& codeTable, vector<int>& code){
		Code aux;
		
		if(t->right == NULL){
			aux.code.assign(code.begin(), code.end());
			aux.e.assign(t->d.first.begin(), t->d.first.end());
			code.clear();
			codeTable.push_back(aux);
		}
		else{
			code.push_back(1);
			generateCodeTable(t->right, codeTable, code);
		}
}

void generateCodeTable(TreeNode* t, vector<Code>& codeTable, vector<int>& code){
		getLeft(t, codeTable, code);
		getRight(t, codeTable, code);
}
/*
void printCode(Code code){
		printf("\n Elemento:");
		for(int i=0; i<code.e.size(); i++){
			printf("%d", code.e[i]);
		}
		
		printf("\tCodigo:");
		for(int i=0; i<code.code.size(); i++){
			printf("%d", code.code[i]);
		}
}*/

void FUF::huffmanCompress() {
	// Node vector with frequencias
	// Count the frequencies
    vector<Data> frequencies = countFrequency(sample->data);

    // Mapped!
//    printFrequency(frequencies);

    // Create the tree
    vector<TreeNode> tree;

    for (unsigned int i = 0; i < frequencies.size(); i++) {
//        tree.push_back(TreeNode(frequencies[i]));
    }

	//Inserir a árvore dentro do arquivo
	vector<Code> codeTable;
	vector<int> code;

//	generateCodeTable(&tree[0], codeTable, code);
    // Ordenados!
//    sortTreeNodes(tree);

    // Now, reduce!
//    while (tree.size() > 1) {
//        reduce(tree);
//        cout << "----------------------------------------------------" << endl;
//        printTree(&tree[0], 1);
//    }

//    printTree(&tree[0], 1);
}

void FUF::huffmanDecompress() {

}

