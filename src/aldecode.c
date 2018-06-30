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

    Imagem *original, *teste;

    original = abreImagem("../imagens/HorizontalCentro.bmp",3);
    
    teste = restringeCentro(original);

    /* IDEIA OPCIONAL:
     Podemos usar rotulação de componentes conexos para restringir melhor
    o local onde fica o AL Code!
    
    Imagem *rotulo;
    rotulo = criaImagem(teste->largura, teste->altura, teste->n_canais);

    ComponenteConexo* componentes;
    int n_componentes;
    n_componentes = rotulaFloodFill (teste, &componentes, 4, 4, 16);

    printf ("%d componentes detectados.\n", n_componentes);

    for (int i = 0; i < n_componentes; i++)
        desenhaRetangulo (componentes [i].roi, criaCor (1,1,1), rotulo);
    salvaImagem (rotulo, "../resultados/Rotulo.bmp");
    free (componentes);
    */

    salvaImagem(teste, "../resultados/Teste.png");
    destroiImagem(original);
    destroiImagem(teste);
    return 0;
}


Imagem *restringeCentro(Imagem *in){
    int prop;
    Imagem *out;
    if(in->largura > in->altura){
        prop = in->largura;
        out = criaImagem(in->largura - 2*prop/4, in->altura - prop/5 + 1, 1);
        for (int j = prop/10; j < in->altura - prop/10; j++)
            for (int k = prop/4; k < in->largura - prop/4; k++)
                if ((in->dados[0][j][k] + in->dados[1][j][k] + in->dados[2][j][k])/3 > 0.5f)
                    out->dados[0][j-prop/10][k-prop/4] = 1.0f;
                else
                    out->dados[0][j-prop/10][k-prop/4] = 0.0f;
    }
    else{
        prop = in->altura;
        out = criaImagem(in->largura - prop/5 + 1, in->altura - 2*prop/4, 1);
        for (int j = prop/4; j < in->altura - prop/4; j++)
            for (int k = prop/10; k < in->largura - prop/10; k++)
                if((in->dados[0][j][k] + in->dados[1][j][k] + in->dados[2][j][k])/3 > 0.5f)
                    out->dados[0][j-prop/4][k-prop/10] = 1.0f;
                else
                    out->dados[0][j-prop/4][k-prop/10] = 0.0f;
    }
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