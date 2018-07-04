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

    Imagem *original, *centro, *code, *rotacionada;

    original = abreImagem("../imagens/HorizontalCentro.bmp",3);

    //centro = atribuiImagemCinza(original);

    centro = restringeCentro(original);
    salvaImagem(centro, "../resultados/Centro.png");
    
    // IDEIA OPCIONAL:
    // Podemos usar rotulação de componentes conexos para restringir melhor
    //o local onde fica o AL Code!
    code = restringeFloodFill(centro);
    salvaImagem(code, "../resultados/Code.png");
    
    rotacionada = rotaciona(code);
    salvaImagem(rotacionada, "../resultados/Rotacionada.png");

    decodifica(rotacionada);

    destroiImagem(code);
    destroiImagem(original);
    destroiImagem(centro);
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

Imagem *atribuiImagemCinza(Imagem *in, float limiar){
    Imagem *out;
    out = criaImagem(in->largura, in->altura, 1);
    for (int j = 0; j < in->altura; j++)
        for (int k = 0; k < in->largura; k++)
            if ((0.299f * in->dados[0][j][k] + 0.587f * in->dados[1][j][k] + 0.114f * in->dados[2][j][k]) > limiar)
                out->dados[0][j][k] = 1.0f;
            else
                out->dados[0][j][k] = 0.0f;
    return out;
}

Imagem *restringeFloodFill (Imagem *centro) {
    Imagem *ALres, *cinza, *rotulo;

    cinza = atribuiImagemCinza(centro, 0.5f);
    salvaImagem(cinza, "../resultados/teste.bmp");
    
    rotulo = criaImagem(centro->largura, centro->altura, centro->n_canais);

    ComponenteConexo* componentes, maiorComp;
    int n_componentes, maiorCoordX = 0, maiorCoordY = 0;
    n_componentes = rotulaFloodFill (cinza, &componentes, 4, 4, 16);

    printf ("%d componentes detectados.\n", n_componentes);

    for (int i = 0; i < n_componentes; i++){
        printf("baixo - cima: %d\n", componentes[i].roi.b - componentes[i].roi.c);
        printf("direita - esquerda: %d\n", componentes[i].roi.d - componentes[i].roi.e);
        printf("diferença: %f\n\n", (float)(componentes[i].roi.b - componentes[i].roi.c)/(float)(componentes[i].roi.d - componentes[i].roi.e));
        if((float)(componentes[i].roi.b - componentes[i].roi.c)/(float)(componentes[i].roi.d - componentes[i].roi.e) < 1.3f && (float)(componentes[i].roi.b - componentes[i].roi.c)/(float)(componentes[i].roi.d - componentes[i].roi.e) > 0.7f)
            desenhaRetangulo (componentes [i].roi, criaCor (1,0,0), rotulo);
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
                ALres->dados[k][j][i] = centro->dados[k][j+maiorComp.roi.c][i+maiorComp.roi.e];

    destroiImagem(cinza);
    free (componentes);
    return ALres;
}

Imagem *rotaciona(Imagem *in) {
    Imagem *out;

    int cont, achou = 0;
    int passos = in->largura/32; 
    int posHX[12], posHY[12];
    out = criaImagem(in->largura, in->altura, in->n_canais);
    copiaConteudo(in,out);

    for (int i = 0; i < out->n_canais; i++){
        for (int j = 0; j < out->altura; j += passos){
            for (int k = 0; k < out->largura - 6*passos; k += passos){
                cont = 0;
                if(out->dados[i][j][k + 0*passos] < 0.5f 
                        && out->dados[i][j][k + 2*passos] < 0.5f 
                        && out->dados[i][j][k + 3*passos] < 0.5f 
                        && out->dados[i][j][k + 4*passos] < 0.5f
                        && out->dados[i][j][k + 6*passos] < 0.5f)
                    cont += 5;
                if (out->dados[i][j][k + passos] > 0.5f
                        && out->dados[i][j][k + 5*passos] > 0.5f)
                    cont += 2;

                if(cont == 7){
                    posHX[achou] = k;
                    posHY[achou] = j;
                    achou++;
                    out->dados[0][j][k] = 0.0f;
                    out->dados[1][j][k] = 1.0f;
                    out->dados[2][j][k] = 1.0f;
                }
            }
        }
    }
    for (int i = 0; i < achou; i++)
        printf("posX(%d) = %d\nposY(%d) = %d\n", i, posHX[i], i, posHY[i]);
    /*for (int i = 0; i < out->n_canais; i++){
        for (int j = 0; j < out->largura; j += passos){
            for (int k = 0; k < out->altura - 6*passos; k += passos){
                cont = 0;
                if(out->dados[i][k + 0*passos][j] < 0.5f 
                        && out->dados[i][k + 2*passos][j] < 0.5f 
                        && out->dados[i][k + 3*passos][j] < 0.5f 
                        && out->dados[i][k + 4*passos][j] < 0.5f
                        && out->dados[i][k + 6*passos][j] < 0.5f)
                    cont += 5;
                if (out->dados[i][k + passos][j] > 0.5f
                        && out->dados[i][k + 5*passos][j] > 0.5f)
                    cont += 2;

                if(cont == 7){
                    achou++;
                    out->dados[0][k][j] = 0.0f;
                    out->dados[1][k][j] = 1.0f;
                    out->dados[2][k][j] = 1.0f;
                }
            }
        }
    }

    for (int i = 0; i < out->n_canais; i++){
        for (int j = 0; j < out->altura - 6*passos; j += passos){
            for (int k = 0; k < out->largura - 6*passos; k += passos){
                cont = 0;
                if(out->dados[i][j + 0*passos][k + 0*passos] < 0.5f 
                        && out->dados[i][j + 2*passos][k + 2*passos] < 0.5f 
                        && out->dados[i][j + 3*passos][k + 3*passos] < 0.5f 
                        && out->dados[i][j + 4*passos][k + 4*passos] < 0.5f
                        && out->dados[i][j + 6*passos][k + 6*passos] < 0.5f)
                    cont += 5;
                if (out->dados[i][j + passos][k + passos] > 0.5f
                        && out->dados[i][j + 5*passos][k + 5*passos] > 0.5f)
                    cont += 2;

                if(cont == 7){
                    achou++;
                    out->dados[0][j][k] = 0.0f;
                    out->dados[1][j][k] = 1.0f;
                    out->dados[2][j][k] = 1.0f;
                }
            }
        }
    }*/

    printf("Achou: %d padroes!\n", achou);

    return out;
}

void decodifica(Imagem *in){
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
}
