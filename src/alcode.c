#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "pdi.h"
#include "alcode.h"

/*  
	int messageToBinary(blocks **b, char *message)

	Função para transformar cada caracter da mensagem em binário.
	Recebe como parâmetro um vetor de estruturas "blocks" (blocks **b) e um vetor
	de caracteres (char *message) e retorna o tamanho final da estrutura.
*/
int messageToBinary(blocks **b, char *message) {
	int aux;
	unsigned int size = (unsigned) strlen(message);
	*b = malloc(size * sizeof(blocks));

	for(unsigned int i = 0; i < size; i += 1) {
		aux = message[i];
		((*b)[i]).decimalValue = (unsigned) aux;		

		int base = 128;
		for(int j = 0; j < 8; j += 1) {
			if(aux - base > -1) {
				aux -= base;
				((*b)[i]).binaryValue[j] = true;
			}
			base /= 2;
		}
	}

	return size;
}

/*  
	void binaryToDecimal(blocks **b, int size)

	Função para ser utilizada após decodificação, onde cada bloco apenas
	contém o valor binário. Transforma o vetor (valor binário) em decimal.
*/
void binaryToDecimal(blocks **b, int size) {
	int base;
	for(int i = 0; i < size; i += 1) {
		((*b)[i]).decimalValue = 0;
		base = 128;
		for(int cont = 0; cont < 8; cont += 1) {
			if(((*b)[i]).binaryValue[cont] == true) {
				((*b)[i]).decimalValue += base;
			}
			base /= 2;
		}
	}
}

/*  
	bool ** initALcode(int ALsize, blocks **b, int bsize)

	Função que retorna a matriz do ALcode dinâmicamente alocada, que é um quarteto da imagem final.
	Recebe como parâmetro o tamanho que a matriz terá (int ALsize), a mensagem a ser inserida (blocks **b)
	e o tamanho da mensagem (int bsize).	
*/
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

	for(int i = 0; i < 8 && bsize > i; i += 1) {		
		for(int j = 0; j < 8; j += 1) {
				matrix[i][j + 8] = ((*b)[i]).binaryValue[j];
		}	
	}

	for(int i = 8; i < ALsize && bsize > i; i += 1) {		
		for(int j = 0; j < 8; j += 1) {
			matrix[i][j] = ((*b)[i]).binaryValue[j];
		}		
	}

	for(int i = 8; i < ALsize && bsize > i + 8; i += 1) {		
		for(int j = 0; j < 8; j += 1) {
			matrix[i][j + 8] = ((*b)[i + 8]).binaryValue[j];
		}		
	}
	
	return matrix;
}

/*  
	void printMatrix(bool **matrix, int size)

	Imprime uma matriz, recebe como parâmetro uma matriz quadrada booleana (bool **matrix)
	e o seu tamanho (int size) e imprime todos os seus elementos.
*/
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

/* 
	void printBlocks(blocks **b, int size)

	Imprime um vetor de estruturas "blocks", recebe como parâmetro 
	os blocos (blocks **b) e quantos deles são (int size)
	e imprime todos os seus elementos.
*/
void printBlocks(blocks **b, int size) {
	if(b) {
		for(int i = 0; i < size; i += 1) {
			printf("\n");
			printf("Char value: %c\n", ((*b)[i]).decimalValue);
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

/*	
	bool ** transpose(bool **matrix, int size)

	Função que recebe como parâmetro uma matriz quadrada booleana (bool **matrix) e 
	o seu tamanho (int size) e retorna a "transposta" dela, onde a primeira linha da 
	matriz de entrada vira a última coluna da matriz de saída.
*/
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

/*  
	void matrixToImage(Imagem *ALCimg, bool **ALCmatrix)

	Função que "transcreve" a matriz do ALcode para uma imagem.
	Recebe como parâmetro a imagem (Imagem *ALCimg) e a matriz (bool **ALCmatrix).
*/
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
