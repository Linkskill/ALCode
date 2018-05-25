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

	Imagem *img;
	int rando1, rando2;
	img = abreImagem(nome, 1);
	img = criaImagem(50,50,1);

	for (int j = 0; j < 50; j += 1) {
		for (int i = 0; i < 50; i += 1) {
			rando1 = rand()%50;
			rando2 = rand()%50;
			// Aqui é onde vai ficar a informação!!!
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

	salvaImagem (img, nome);

	destroiImagem(img);

	return 0;
}