#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "pdi.h"

/** Cria o AL Code **/
#include "alcode.h"

/** Decodificação da informação na imagem **/
#include "aldecode.h"

int main () {
	
	char *nome = {"../resultados/ALCode.png"};
	char *nomeP = {"../resultados/ALCPic.png"};
	blocks *binaryMessage;
	int bsize;
	bsize = messageToBinary(&binaryMessage, "abcdefghijklmnopqrstuvwxyz");
	//printBlocks(&binaryMessage, bsize);
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
