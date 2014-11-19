#include "FUF.h"
#include <stdio.h>
#include <vector>
#include "Utils.h"
#include "WaveReader.h"

using namespace std;
/*
//Return the frenquency in order of each channel
int** frequencyInOrder(int* channel, int dataLength){
	vector< vector<int> > frequency;
	int i = 0, j = 0, aux, count = 0;
	
	while( j < dataLength ) {
		aux = channel(j);
		for(i=0;i<dataLength;i++) {
					if(aux == channel(i)) count++;
		}
		if(frenquency.size == 0){ 
			frequency(j)(0).push_back(aux);
			frequency(j)(1).push_back(count);
		}
		else {
		}
		j++;
	}
} */

void buildTree(WaveReader sample){
 
}

void FUF::huffmanCompress() {
	buildTree(sample);
	printWavInfo(sample.info);
}

