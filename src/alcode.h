#ifndef __ALCODE_H
#define __ALCODE_H

#include "pdi.h"

typedef struct {
	unsigned int decimalValue;
	bool binaryValue[8];
} blocks;

/** Criação do ALCode **/
int messageToBinary(blocks **b, char *message);
bool ** initALcode(int ALsize, blocks **b, int bsize);
void printMatrix(bool **matrix, int size);
void printBlocks(blocks **b, int size);
bool ** transpose(bool **matrix, int size);
void matrixToImage(Imagem *ALCimg, bool **ALCmatrix);

#endif