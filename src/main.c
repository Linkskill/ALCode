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

int messageToBinary(blocks **b, char *message);
bool ** initALcode(int ALsize, blocks **b, int bsize);
void printMatrix(bool **matrix, int size);
void printBlocks(blocks **b, int size);
void matrixToImage(Imagem *ALCimg, bool **ALCmatrix);

int main () {
	
	char *nome = {"../imagens/ALCode.png"};
	char *nomeP = {"../imagens/ALCPic.png"};
	blocks *binaryMessage;
	int bsize;
	bsize = messageToBinary(&binaryMessage, "abcdefghijklmnopqrstuvwxyz");
	Imagem *ALCimg, *ALCpic;
	
	int ALsize = 16;
	bool **ALcode = initALcode(ALsize, &binaryMessage, bsize);
	printMatrix(ALcode, ALsize);

	ALCimg = abreImagem(nome, 1);
	ALCimg = criaImagem(2*ALsize, 2*ALsize, 1);

	ALCpic = abreImagem(nomeP, 1);
	ALCpic = criaImagem(16*ALsize, 16*ALsize, 1);

	matrixToImage(ALCimg, ALcode);
	redimensionaNN(ALCimg, ALCpic);

	salvaImagem(ALCimg, nome);
	salvaImagem(ALCpic, nomeP);

	destroiImagem(ALCimg);
	destroiImagem(ALCpic);
	return 0;
}

int messageToBinary(blocks **b, char *message) {
	int aux;
	unsigned int size = (unsigned) strlen(message);
	*b = malloc(size * sizeof(blocks));

	for(unsigned int i = 0; i < size; i += 1) {
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

bool ** initALcode(int ALsize, blocks **b, int bsize) {

	if(ALsize < 7) {
		printf("Erro, tamanho deve ser no mínimo 7.\n");
		return NULL;
	}

	bool **matrix = malloc(ALsize * sizeof(bool *));
	for(int i = 0; i < ALsize; i += 1) matrix[i] = malloc(ALsize * sizeof(bool));

	for(int i = 1; i < 6; i += 1) {
		matrix[1][i] = 1;
		matrix[5][i] = 1;
		matrix[i][1] = 1;
		matrix[i][5] = 1;
	}

	for(int i = 0; i < 8; i += 1) {
		matrix[7][i] = 1;
		matrix[i][7] = 1;
	}

	for(int i = 0; i < 8; i += 1) {
		for(int j = 0; j < 8; j += 1) {
			matrix[i][j + 8] = ((*b)[i]).binaryValue[j];
		}
	}

	for(int i = 8; i < ALsize; i += 1) {
		for(int j = 0; j < 8; j += 1) {
			matrix[i][j] = ((*b)[i]).binaryValue[j];
		}
	}

	for(int i = 8; i < ALsize; i += 1) {
		for(int j = 0; j < 8; j += 1) {
			matrix[i][j + 8] = ((*b)[i + 8]).binaryValue[j];
		}
	}

	/* if(bsize > 8) {
		for(int i = 0; i < )
	} */
	
	return matrix;
}

void printMatrix(bool **matrix, int size) {
	if(matrix) {
		for(int i = 0; i < size; i += 1) {
			for(int j = 0; j < size; j += 1) {			
				printf("%d ", matrix[i][j]);
			}
			printf("\n");
		}
	}
	else
		printf("Erro, a matriz nao existe.\n");
}

void printBlocks(blocks **b, int size) {
	if(b) {
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
	else
		printf("Erro, os blocos nao existem.");
}

void matrixToImage(Imagem *ALCimg, bool **ALCmatrix) {
	for(int i = 0; i < ALCimg->altura/2; i += 1) {
		for(int j = 0; j < ALCimg->largura/2; j += 1) {
			if(ALCmatrix[i][j])
				ALCimg->dados[0][i][j] = 1.0f;
			else
				ALCimg->dados[0][i][j] = 0.0f;
		}
	}
	
	for(int i = 0; i < ALCimg->altura/2; i += 1) {
		for(int j = ALCimg->largura/2; j < ALCimg->largura; j += 1) {
			if(ALCmatrix[i][ALCimg->largura - j - 1])
				ALCimg->dados[0][i][j] = 1.0f;
			else
				ALCimg->dados[0][i][j] = 0.0f;
		}
	}
	
	for(int i = ALCimg->altura/2; i < ALCimg->altura; i += 1) {
		for(int j = 0; j < ALCimg->largura/2; j += 1) {
			if(ALCmatrix[ALCimg->altura - i - 1][j])
				ALCimg->dados[0][i][j] = 1.0f;
			else
				ALCimg->dados[0][i][j] = 0.0f;
		}
	}
	
	for(int i = ALCimg->altura/2; i < ALCimg->altura; i += 1) {
		for(int j = ALCimg->largura/2; j < ALCimg->largura; j += 1) {
			if(ALCmatrix[ALCimg->altura - i - 1][ALCimg->largura - j - 1])
				ALCimg->dados[0][i][j] = 1.0f;
			else
				ALCimg->dados[0][i][j] = 0.0f;
		}
	}
}




