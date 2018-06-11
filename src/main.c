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
bool ** transpose(bool **matrix, int size);
void matrixToImage(Imagem *ALCimg, bool **ALCmatrix);

int main () {
	
	char *nome = {"../resultados/ALCode.png"};
	char *nomeP = {"../resultados/ALCPic.png"};
	blocks *binaryMessage;
	int bsize;
	bsize = messageToBinary(&binaryMessage, "abserglajdlckejtlsjcjgls");
	Imagem *ALCimg, *ALCpic;
	
	int ALsize = 16;
	bool **ALcode = initALcode(ALsize, &binaryMessage, bsize);

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
		if(bsize > i) {
			for(int j = 0; j < 8; j += 1) {
					matrix[i][j + 8] = ((*b)[i]).binaryValue[j];
			}
		}
		else
			break;
	}

	for(int i = 8; i < ALsize; i += 1) {
		if(bsize > i) {
			for(int j = 0; j < 8; j += 1) {
				matrix[i][j] = ((*b)[i]).binaryValue[j];
			}
		}
		else
			break;
	}

	for(int i = 8; i < ALsize; i += 1) {
		if(bsize > i + 8) {
			for(int j = 0; j < 8; j += 1) {
				matrix[i][j + 8] = ((*b)[i + 8]).binaryValue[j];
			}
		}
		else
			break;
	}
	
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

bool ** transpose(bool **matrix, int size) {
	bool **transpose = malloc(size * sizeof(bool *));
	for(int i = 0; i < size; i += 1) transpose[i] = malloc(size * sizeof(bool));

	for(int i = 0; i < size; i += 1) {
		for(int j = 0; j < size; j += 1) {
			transpose[j][size - i - 1] = matrix[i][j];
		}
	} 

	return transpose;
}

void matrixToImage(Imagem *ALCimg, bool **ALCmatrix) {
	int line = 0;
	int column = 0;
	for(int i = 0; i < 4; i += 1) {
		int mline = 0;
		int mcolumn = 0;
		for(int y = line; y < line + ALCimg->altura / 2; y += 1) {
			for(int x = column; x < column + ALCimg->largura / 2; x += 1) {
				if(ALCmatrix[mline][mcolumn])
					ALCimg->dados[0][y][x] = 1.0f;
				else
					ALCimg->dados[0][y][x] = 0.0f;
				mcolumn += 1;
			}
			mcolumn = 0;
			mline += 1;
		}
		if(i == 0)		column += ALCimg->largura / 2;
		else if(i == 1)	line += ALCimg->altura / 2;
		else			column -= ALCimg->largura / 2;
		ALCmatrix = transpose(ALCmatrix, ALCimg->altura / 2);
	}
}
