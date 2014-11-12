#include "FUF.h"
#include <stdio.h>
#include "Utils.h"
#include "WaveReader.h"

using namespace std;

//Return the frenquency in order of each channel
int** frequencyInOrder(int* channel, int dataLength){
	vector< vector<int> > frequency;
	int i = 0, aux, count = 0;
	
	aux = channel(0);
	
	for(i=0;i<dataLength;i++) {
			
	}
	
} 

void buildTree(WaveReader sample){
	vector< 
}

void FUF::huffmanCompress() {
	buildTree(sample);
	printWavInfo(sample.info);
}

