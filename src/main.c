#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "pdi.h"

/** As funções dos outros .h (tipo abreImagem(...,...)) 
 * criaremos aqui posteriormente. **/

int main (){
	srand(time(NULL));
	char *nome = {"../resultados/ALCode.png"};
	//Informação a ser inserida no AL Code -> Hello World!
	//char *msg = {"Hello World!"};
	//Em binário fica:
	long long int msgBin[6] = {100100001100101, 110110001101100, 110111100100000, 
								101011101101111, 111001001101100, 110010000100001};
	//descartando o primeiro zero pq da merda na hora da divisão...
	//o resultado fica o mesmo se tivesse o primeiro zero
	long long int divis = 1000000000000000, divisAux = divis;
	int aux, count = 0;

	//img é o AL Code e imgAux é um mapa pra ver onde vai a informação
	Imagem *img, *imgAux;
	int rando1, rando2;
	img = abreImagem(nome, 1);
	img = criaImagem(50,50,1);

	imgAux = abreImagem("../resultados/Mapa.png", 1);
	imgAux = criaImagem(50,50,1);

	for (int j = 0; j < 50; j += 1) {
		for (int i = 0; i < 50; i += 1) {
			rando1 = rand()%50;
			rando2 = rand()%50;
			// Aqui é onde vai ficar os bit aleatório pra confundir!!!
			if ((i > 7 && i < 42) || (j > 7 && j < 42) || (i > 41 && j > 41)) {
				if((rando1 > 7 && rando1 < 42) || (rando2 > 7 && rando2 < 42) 
						|| (rando1 > 41 && rando2 > 41))
					img->dados[0][rando2][rando1] = 1.0f;
			}
			// Quadrados
			else {
				/** Moldura externa **/
				if (i == 7 || j == 7)
					img->dados[0][j][i] = 1.0f;
				else if (i == 42 || j == 42)
					img->dados[0][j][i] = 1.0f;
				/** Moldura interna **/
				// Quadrado superior esquerdo
				else if ((i == 5 || i == 1) && j > 0 && j < 6)
					img->dados[0][j][i] = 1.0f;
				else if ((j == 5 || j == 1) && i > 0 && i < 6)
					img->dados[0][j][i] = 1.0f;
				// Quadrado superior direito
				else if ((i == 44 || i == 48) && j > 0 && j < 6)
					img->dados[0][j][i] = 1.0f;
				else if ((j == 1 || j == 5) && i > 44 && i < 48)
					img->dados[0][j][i] = 1.0f;
				// Quadrado inferior esquerdo
				else if ((i == 1 || i == 5) && j > 44 && j < 48)
					img->dados[0][j][i] = 1.0f;
				else if ((j == 44 || j == 48) && i > 0 && i < 6)
					img->dados[0][j][i] = 1.0f;
			}	
		}
	}

	//Copia o AL Code no mapa 
	copiaConteudo(img, imgAux);

	/** AQUI É ONDE FICA A INFORMAÇÃO!!!!!!!! **/
	for (int j = 20; j < 30; j += 1) {
		for (int i = 20; i < 30; i += 1) {
			img->dados[0][j][i] = 0.0f;
			if(divisAux > 9 && count < 6){
				aux = (msgBin[count]/divisAux)%10;
				//printf("aux: %d\n", aux);
				if(aux == 1)
					img->dados[0][j][i] = 1.0f;
				else
					img->dados[0][j][i] = 0.0f;
				//printf ("divisAux: %lld\n", divisAux);
				divisAux /= 10;
			}
			else{ 
				divisAux = divis;
				if (count < 6)
					count++;
			}
		}
	}

	//Preenche de cinza no mapa a parte onde vai a informação
	for (int j = 20; j < 30; j += 1) 
		for (int i = 20; i < 30; i += 1) 
			imgAux->dados[0][j][i] = 0.5f;
		
	//Pode ser descartado, soma pra vizualizar melhor onde fica a informação
	soma(img, imgAux, 1, 1, img);

	salvaImagem (img, nome);
	salvaImagem (imgAux, "../resultados/Mapa.png");

	destroiImagem(img);
	destroiImagem(imgAux);

	return 0;
}