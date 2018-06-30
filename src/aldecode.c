#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "pdi.h"

/** Cria o AL Code **/
#include "alcode.h"

/** Cria o AL Decode **/
#include "aldecode.h"

int main(){

    /*
    2 opções de resolução: 
     - Tentar achar os 4 quadrados na imagem, que vai servir como alinhamento
    para ler a região que interessa na imagem (geralmente o centro)
     - Tentar fazer uma janela deslizante (com o padrão do próprio ALcode) 
    que vai aumentando a cada iteração "region growing" e ir comparando cada pixel
    por meio de um threshold 
    */

    Imagem *original, *aux, *ALres, *cinza;

    original = abreImagem("../imagens/HorizontalCentro.bmp",3);

    //aux = atribuiImagemCinza(original);

    aux = restringeCentro(original);

    cinza = atribuiImagemCinza(aux);

    // IDEIA OPCIONAL:
    // Podemos usar rotulação de componentes conexos para restringir melhor
    //o local onde fica o AL Code!
    
    Imagem *rotulo;
    rotulo = criaImagem(aux->largura, aux->altura, aux->n_canais);

    ComponenteConexo* componentes, maiorComp;
    int n_componentes, maiorCoordX = 0, maiorCoordY = 0;
    n_componentes = rotulaFloodFill (cinza, &componentes, 4, 4, 16);

    printf ("%d componentes detectados.\n", n_componentes);

    for (int i = 0; i < n_componentes; i++){
        desenhaRetangulo (componentes [i].roi, criaCor (1,1,1), rotulo);
        if(maiorCoordY < componentes[i].roi.b-componentes[i].roi.c
                && maiorCoordX < componentes[i].roi.d-componentes[i].roi.e) {
            maiorCoordY = componentes[i].roi.b-componentes[i].roi.c;
            maiorCoordX = componentes[i].roi.d-componentes[i].roi.e;
            maiorComp = componentes[i];
        }
    }

    salvaImagem (rotulo, "../resultados/Rotulo.bmp");

    ALres = criaImagem(maiorComp.roi.d - maiorComp.roi.e,
                        maiorComp.roi.b - maiorComp.roi.c,3);

    for (int k = 0; k < ALres->n_canais; k++)
        for (int j = 0; j < ALres->altura; j++)
            for (int i = 0; i < ALres->largura; i++)
                ALres->dados[k][j][i] = aux->dados[k][j+maiorComp.roi.c][i+maiorComp.roi.e];

    salvaImagem(aux, "../resultados/Teste.png");
    salvaImagem(ALres, "../resultados/ALRes.png");
    destroiImagem(ALres);
    destroiImagem(original);
    destroiImagem(aux);
    destroiImagem(cinza);
    free (componentes);
    return 0;
}


Imagem *restringeCentro(Imagem *in){
    int prop;
    Imagem *out;
    if(in->largura > in->altura){
        prop = in->largura;
        out = criaImagem(in->largura - 2*prop/4, in->altura - prop/5 + 1, 3);
        for(int i = 0; i < in->n_canais; i++)
            for (int j = prop/10; j < in->altura - prop/10; j++)
                for (int k = prop/4; k < in->largura - prop/4; k++)
                    out->dados[i][j-prop/10][k-prop/4] = in->dados[i][j][k];
    }
    else{
        prop = in->altura;
        out = criaImagem(in->largura - prop/5 + 1, in->altura - 2*prop/4, 3);
        for(int i = 0; i < in->n_canais; i++)
            for (int j = prop/4; j < in->altura - prop/4; j++)
                for (int k = prop/10; k < in->largura - prop/10; k++)
                    out->dados[i][j-prop/4][k-prop/10] = in->dados[i][j][k];
    }
    return out;
}

Imagem *atribuiImagemCinza(Imagem *in){
    Imagem *out;
    out = criaImagem(in->largura, in->altura, 1);
    for (int j = 0; j < in->altura; j++)
        for (int k = 0; k < in->largura; k++)
            if ((in->dados[0][j][k] + in->dados[1][j][k] + in->dados[2][j][k])/3 > 0.5f)
                out->dados[0][j][k] = 1.0f;
            else
                out->dados[0][j][k] = 0.0f;
    return out;
}


/*
    Imagem *img;
    img = abreImagem("../resultados/ALCode.png",1);
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
    destroiImagem(img);
*/