#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "pdi.h"

typedef struct {
	unsigned int decimalValue;
	bool binaryValue[8];
} blocks;

/** As funções dos outros .h (tipo abreImagem(...,...)) 
 * criaremos aqui posteriormente. **/
int messageToBinary(blocks **b, char *message);
void printBlocks(blocks **b, int size);

int main () {
	
	blocks *binaryMessage;
	int size;
	size = messageToBinary(&binaryMessage, "abcdefghijklmnopqrstuvwxyz");
	printBlocks(&binaryMessage, size);

	return 0;
}

int messageToBinary(blocks **b, char *message) {
	int aux;
	unsigned int size = (unsigned) strlen(message);
	*b = malloc(size * sizeof(blocks));

	for(int i = 0; i < size; i += 1) {
		aux = message[i];
		((*b)[i]).decimalValue = (unsigned) aux;		

		if(aux - 128 > -1) {
			aux -= 128;
			((*b)[i]).binaryValue[0] = true;
		}
		if(aux - 64 > -1) {
			aux -= 64;
			((*b)[i]).binaryValue[1] = true;
		}
		if(aux - 32 > -1) {
			aux -= 32;
			((*b)[i]).binaryValue[2] = true;
		}
		if(aux - 16 > -1) {
			aux -= 16;
			((*b)[i]).binaryValue[3] = true;
		}
		if(aux - 8 > -1) {
			aux -= 8;
			((*b)[i]).binaryValue[4] = true;
		}
		if(aux - 4 > -1) {
			aux -= 4;
			((*b)[i]).binaryValue[5] = true;
		}
		if(aux - 2 > -1) {
			aux -= 2;
			((*b)[i]).binaryValue[6] = true;
		}
		if(aux - 1 > -1) {
			aux -= 1;
			((*b)[i]).binaryValue[7] = true;
		}
	}

	return size;
}

bool ** initCode(int size) {

}

void printBlocks(blocks **b, int size) {
	for(int i = 0; i < size; i += 1) {
		printf("Decimal value: %d\n", ((*b)[i]).decimalValue);
		printf("Binary value: ");
		for(int j = 0; j < 8; j += 1) {
			if (((*b)[i]).binaryValue[j]) printf("1");
			else printf("0");
		}
		printf("\n");
	}
}




