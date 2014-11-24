#include "FUF.h"
#include <stdio.h>
#include <vector>
#include "Utils.h"
#include "WaveReader.h"

using namespace std;

/*
Estrutura para a fila de frequências em ordem
crescente.
*/
struct frequency{
    int number;
    int freq;
};

/*
Estrutura para os nós internos da árvore.
*/
struct treeNode{
    treeNode* left;
    treeNode* right;
    frequency element;

    const bool operator < (const treeNode& t) const{
        return this->element.freq < t.element.freq;
    }
};

vector<treeNode> getFrequency(int* sequencia, int n){
    map<int, int> mySet;
    vector<treeNode> heap;

    for(int i=0;i<n;i++){
        if (mySet.count(sequencia[i])) {
            mySet[sequencia[i]]++;
        }
        else {
            mySet[sequencia[i]] = 1;
        }
    }

    map<int, int>::iterator it = mySet.begin();

    for(it = mySet.begin();it!=mySet.end();it++){
        treeNode f;
        f.element.number = it->first;
        f.element.freq = it->second;
        f.left = NULL;
        f.right = NULL;
        heap.push_back(f);
    }

    return heap;
}

/*
Funcao só para teste. Depois que terminar o trabalho,
pode ser excluída.
*/
void imprimir(vector<treeNode> heap){
    for(int i=0; i<heap.size();i++){
        printf("\nElemento %2d: %2d", i, heap[i].element.number);
        printf("  Frequencia: %2d", heap[i].element.freq);
    }
}

void orderByFrequency(vector<treeNode>& heap){
    sort(heap.begin(), heap.end());
}

vector<treeNode> insertFrequency(vector<treeNode> heap, treeNode newElement){
    heap.push_back(newElement);
    orderByFrequency(heap);
    return heap;
}

vector<treeNode> removeFrequency(vector<treeNode> heap, treeNode newElement){
    for(int i=0;i<heap.size();i++){
        if(newElement.element.number == heap[i].element.number){
                heap.erase(heap.begin()+i);
        }
    }

    return heap;
}

treeNode createElement(vector<treeNode>& heap, treeNode right, treeNode left){
    treeNode newElement;

    newElement.element.freq = right.element.freq + left.element.freq;
    newElement.element.number = 0;
    newElement.left = &left;
    newElement.right = &right;
		

    heap = removeFrequency(heap, right);
    heap = removeFrequency(heap, left);
    heap = insertFrequency(heap, newElement);

    return newElement;
}

treeNode createHuffmanTree(vector<treeNode> heap){
    treeNode auxTree;
    int i = 0;

    while(heap.size()!= 1){
        auxTree = createElement(heap, heap[1], heap[0]);
        //printf("\n Iteracao %d:", i);
        //imprimir(heap);
        i++;
    }

    return auxTree;
}


void buildTree(WaveReader sample){

	//Heap = fila de frequências em ordem crescente
	vector<treeNode> heap;
	treeNode tree;
	vector<treeNode> treeVector;
	
	//Árvore de Huffman
	treeNode tree;
	
	
	for(int i=0; i<sample.data.channelCount; i++){
		heap = getFrequency(sample.data.data[i], sample.data.dataLength);
		orderByFrequency(heap);
		tree = createHuffmanTree(heap);
		treeVector.push_back(tree);
	}
	
	
}

void FUF::huffmanCompress() {
	printWavInfo(sample.info);
	
	
}

void FUF::huffmanDecompress() {}
