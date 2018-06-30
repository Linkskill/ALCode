#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "pdi.h"

/** Cria o AL Code **/
#include "alcode.h"

Imagem *restringeCentro(Imagem *in);

int main(){

    /*
    2 opções de resolução: 
     - Tentar achar os 4 quadrados na imagem, que vai servir como alinhamento
    para ler a região que interessa na imagem (geralmente o centro)
     - Tentar fazer uma janela deslizante (com o padrão do próprio ALcode) 
    que vai aumentando a cada iteração "region growing" e ir comparando cada pixel
    por meio de um threshold 
    */

    Imagem *img, *original, *teste;

    img = abreImagem("../resultados/ALCode.png",1);
    original = abreImagem("../imagens/HorizontalCentro.bmp",3);
    
    teste = restringeCentro(original);

    if(!img){
        printf("No image found!\n");
        return -1;
    }

    int mat[img->largura/2][img->altura/2];

    for(int i = 0; i < img->largura/2; i += 1){
        for(int j = 0; j < img->altura/2; j += 1){
            if(img->dados[0][i][j] > 0.5f){
                mat[i][j] = 1;
                printf("%2d", mat[i][j]);
            }
            else{
                mat[i][j] = 0;
                printf("%2d", mat[i][j]);
            }
        }
        printf("\n");
    }

    salvaImagem(teste, "../resultados/Teste.png");

    destroiImagem(img);
    destroiImagem(original);
    destroiImagem(teste);
    return 0;
}


Imagem *restringeCentro(Imagem *in){
    int prop;
    Imagem *out;
    if(in->largura > in->altura){
        prop = in->largura;
        out = criaImagem(in->largura - 2*prop/4, in->altura - prop/5 + 1, in->n_canais);
        for (int i = 0; i < in->n_canais; i++)
            for (int j = prop/10; j < in->altura - prop/10; j++)
                for (int k = prop/4; k < in->largura - prop/4; k++)
                    out->dados[i][j-prop/10][k-prop/4] = in->dados[i][j][k];
    }
    else{
        prop = in->altura;
        out = criaImagem(in->largura - prop/5 + 1, in->altura - 2*prop/4, in->n_canais);
        for (int i = 0; i < in->n_canais; i++)
            for (int j = prop/4; j < in->altura - prop/4; j++)
                for (int k = prop/10; k < in->largura - prop/10; k++)
                    out->dados[i][j-prop/4][k-prop/10] = in->dados[i][j][k];
    }
    return out;
}