#ifndef UTILS_H
#define UTILS_H

#include "WaveReader.h"
#include "Huffman.h"
#include <string>

/**
 *  This is an util library.
 *  Functions for general use should come here.
 */

/**
 *  Print wav header info
 */
void printWavInfo(WaveInfo);

/**
 *  Print wav data
 */
void printWavData(WaveData);


/**
 *  Print frequency vector
 */
void printFrequency(vector<Data> f);

/**
 *  Print Huffman tree
 */
void printTree(TreeNode* t, int depth = 1, string code = "");


#endif // UTILS_H
