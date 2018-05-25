#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pdi.h"

int main (){
	char *nome = {"../resultados/ALCode.png"};

	Imagem *img;
	Retangulo ret, bord;

	img = abreImagem(nome, 1);

	img = criaImagem(50,50,1);

	//bord = criaRetangulo(0,8,0,8);
	//desenhaRetangulo(bord, criaCor(1,1,1), img);
	ret = criaRetangulo(1,5,1,5);
	desenhaRetangulo(ret, criaCor(1,1,1), img);

	salvaImagem (img, nome);

	destroiImagem(img);

	return 0;
}