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

    Imagem *original, *centro, *code, *rotacionada, *binarizada;

    original = abreImagem("../imagens/HorizontalCentro.bmp",3);

    //centro = atribuiImagemCinza(original);

    centro = criaImagem(original->largura,original->altura,original->n_canais);
//    copiaConteudo(original,centro);
    centro = restringeCentro(original);
    salvaImagem(centro, "../resultados/Centro.png");

    binarizada = atribuiImagemCinza(centro);
    salvaImagem(binarizada, "../resultados/Binarizada.png");
    // IDEIA OPCIONAL:
    // Podemos usar rotulação de componentes conexos para restringir melhor
    //o local onde fica o AL Code!
    code = restringeFloodFill(binarizada);
    salvaImagem(code, "../resultados/Code.png");

    rotacionada = rotaciona(code);
    salvaImagem(rotacionada, "../resultados/Rotacionada.png");

    decodifica(rotacionada);

    destroiImagem(original);
    destroiImagem(centro);
    destroiImagem(binarizada);
    destroiImagem(code);
    destroiImagem(rotacionada);
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

Imagem *restringeFloodFill (Imagem *in) {
    Imagem *ALres, *auxiliar, *rotulo;

    auxiliar = criaImagem(in->largura, in->altura, in->n_canais);
    copiaConteudo(in,auxiliar);
//    auxiliar = atribuiImagemCinza(in);
    
    rotulo = criaImagem(auxiliar->largura, auxiliar->altura, auxiliar->n_canais);

    ComponenteConexo* componentes, maiorComp;
    int n_componentes, maiorCoordX = 0, maiorCoordY = 0;
    n_componentes = rotulaFloodFill (auxiliar, &componentes, 4, 4, 16);

    printf ("%d componentes detectados.\n", n_componentes);

    for (int i = 0; i < n_componentes; i++){
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
                        maiorComp.roi.b - maiorComp.roi.c, 1);

    for (int j = 0; j < ALres->altura; j++)
        for (int i = 0; i < ALres->largura; i++)
            ALres->dados[0][j][i] = in->dados[0][j+maiorComp.roi.c][i+maiorComp.roi.e];

    destroiImagem(auxiliar);
    free (componentes);
    return ALres;
}

Imagem *rotaciona(Imagem *in) {
    Imagem *out;

    int cont, achouV = 0, achouH = 0;
    int passos = in->largura/32; 
    int posHX[12], posHY[12];
    int posHXI[12], posHYI[12];
    int posVX[12], posVY[12];
    int posVXI[12], posVYI[12];
    int centroV[4], centroH[4];
    out = criaImagem(in->largura, in->altura, in->n_canais);
    copiaConteudo(in,out);

    
    // Percorre verticalmente de cima para baixo
    for (int j = 0; j < out->largura; j += passos){
        for (int k = 0; k < out->altura - 6*passos; k += passos){
            cont = 0;
            if(out->dados[0][k + 0*passos][j] < 0.5f 
                    && out->dados[0][k + 2*passos][j] < 0.5f 
                    && out->dados[0][k + 3*passos][j] < 0.5f 
                    && out->dados[0][k + 4*passos][j] < 0.5f
                    && out->dados[0][k + 6*passos][j] < 0.5f)
                cont += 5;
            if (out->dados[0][k + passos][j] > 0.5f
                    && out->dados[0][k + 5*passos][j] > 0.5f)
                cont += 2;

            if(cont == 7){
                posVX[achouV] = j;
                posVY[achouV] = k;
                achouV++;
                out->dados[0][k][j] = 1.0f;              
//                for (int i = 0; i < 6*passos; i++)
//                    out->dados[0][k+i][j] = 1.0f;
            }
        }
    }

    for (int i = 0; i < achouV; i++){
        printf("Vertical: posX(%d) = %d\nposY(%d) = %d\n", i, posVX[i], i, posVY[i]);
    }

    achouV = 0;
    // Percorre verticalmente de baixo para cima
    for (int j = out->largura - 1; j >= 0; j -= passos){
        for (int k = out->altura - 1; k >= 6*passos; k -= passos){
            cont = 0;
            if(out->dados[0][k - 0*passos][j] < 0.5f 
                    && out->dados[0][k - 2*passos][j] < 0.5f 
                    && out->dados[0][k - 3*passos][j] < 0.5f 
                    && out->dados[0][k - 4*passos][j] < 0.5f
                    && out->dados[0][k - 6*passos][j] < 0.5f)
                cont += 5;
            if (out->dados[0][k - passos][j] > 0.5f
                    && out->dados[0][k - 5*passos][j] > 0.5f)
                cont += 2;

            if(cont == 7){
                posVXI[achouV] = j;
                posVYI[achouV] = k;
                achouV++;
                out->dados[0][k][j] = 1.0f;
//                for (int i = 0; i < 6*passos; i++)
//                    out->dados[0][k-i][j] = 0.0f;
            }
        }
    }

   for (int i = 0; i < achouV; i++){
        printf("Vertical: posX(%d) = %d\nposY(%d) = %d\n", i, posVXI[i], i, posVYI[i]);
    }

    // Percorre horizontalmente da esquerda para a direita
    for (int j = 0; j < out->altura; j += passos){
        for (int k = 0; k < out->largura - 6*passos; k += passos){
            cont = 0;
            if(out->dados[0][j][k + 0*passos] < 0.5f 
                    && out->dados[0][j][k + 2*passos] < 0.5f 
                    && out->dados[0][j][k + 3*passos] < 0.5f 
                    && out->dados[0][j][k + 4*passos] < 0.5f
                    && out->dados[0][j][k + 6*passos] < 0.5f)
                cont += 5;
            if (out->dados[0][j][k + passos] > 0.5f
                    && out->dados[0][j][k + 5*passos] > 0.5f)
                cont += 2;

            if(cont == 7){
                posHX[achouH] = k;
                posHY[achouH] = j;
                achouH++;
                out->dados[0][j][k] = 1.0f;
//                for (int i = 0; i < 6*passos; i++)
//                    out->dados[0][j][k+i] = 1.0f;
            }
        }
    }

    for (int i = 0; i < achouH; i++){
        printf("Horizontal: posX(%d) = %d\nposY(%d) = %d\n", i, posHX[i], i, posHY[i]);
    }
    
    achouH = 0;
    // Percorre horizontalmente da direita para a esquerda
    for (int j = out->altura - 1; j >= 0; j -= passos){
        for (int k = out->largura - 1; k >= 6*passos; k -= passos){
            cont = 0;
            if(out->dados[0][j][k - 0*passos] < 0.5f 
                    && out->dados[0][j][k - 2*passos] < 0.5f 
                    && out->dados[0][j][k - 3*passos] < 0.5f 
                    && out->dados[0][j][k - 4*passos] < 0.5f
                    && out->dados[0][j][k - 6*passos] < 0.5f)
                cont += 5;
            if (out->dados[0][j][k - passos] > 0.5f
                    && out->dados[0][j][k - 5*passos] > 0.5f)
                cont += 2;

            if(cont == 7){
                posHXI[achouH] = k;
                posHYI[achouH] = j;
                achouH++;
                out->dados[0][j][k] = 1.0f;
//                for (int i = 0; i < 6*passos; i++)
//                    out->dados[0][j][k-i] = 0.5f;
            }
        }
    }
    
    for (int i = 0; i < achouH; i++){
        printf("Horizontal: posX(%d) = %d\nposY(%d) = %d\n", i, posHXI[i], i, posHYI[i]);
    }

    int indice;

    int posDX[4], posDY[4];
    int posDXI[4], posDYI[4];

    for (int i = 0; i < 4; i++){
        posDX[i] = -1;
        posDXI[i] = -1;
        posDY[i] = -1;
        posDYI[i] = -1;
    }

    if(achouH > achouV) 
        indice = achouH;
    else 
        indice = achouV;

    achouH = 0;
    for (int k = 0; k < indice; k++){
        cont = 0;
        if(out->dados[0][posVY[k]][posHX[k]] < 0.5f 
                && out->dados[0][posVY[k] + 2*passos][posHX[k] + 2*passos] < 0.5f
                && out->dados[0][posVY[k] + 3*passos][posHX[k] + 3*passos] < 0.5f
                && out->dados[0][posVY[k] + 4*passos][posHX[k] + 4*passos] < 0.5f
                && out->dados[0][posVY[k] + 6*passos][posHX[k] + 6*passos] < 0.5f)
            cont += 5;
        if(out->dados[0][posVY[k] + passos][posHX[k] + passos] > 0.5f
                && out->dados[0][posVY[k] + 5*passos][posHX[k] + 5*passos] > 0.5f)
            cont += 2;
        
        if(cont == 7){
            out->dados[0][posVY[k]][posHX[k]] = 1.0f;
            posDX[achouH] = posHX[k];
            posDY[achouH] = posVY[k];
            achouH++;
        }
    }
    
    achouV = 0;
    for (int k = 0; k < indice; k++){
        cont = 0;
        if(out->dados[0][posVYI[k]][posHXI[k]] < 0.5f 
                && out->dados[0][posVYI[k] - 2*passos][posHXI[k] - 2*passos] < 0.5f
                && out->dados[0][posVYI[k] - 3*passos][posHXI[k] - 3*passos] < 0.5f
                && out->dados[0][posVYI[k] - 4*passos][posHXI[k] - 4*passos] < 0.5f
                && out->dados[0][posVYI[k] - 6*passos][posHXI[k] - 6*passos] < 0.5f)
            cont += 5;
        if(out->dados[0][posVYI[k] - passos][posHXI[k] - passos] > 0.5f
                && out->dados[0][posVYI[k] - 5*passos][posHXI[k] - 5*passos] > 0.5f)
            cont += 2;
        
        if(cont == 7){
            out->dados[0][posVYI[k]][posHXI[k]] = 1.0f;
            posDXI[achouV] = posHXI[k];
            posDYI[achouV] = posVYI[k];
            achouV++;
        }
    }   

    for (int k = 0; k < achouH; k++){
        for (int l = 0; l < achouV; l++){
            if(abs(posDX[k] - posDXI[l]) < out->largura/2
                    && abs(posDY[k] - posDYI[l]) < out->altura/2){
                centroH[cont] = (posDX[k] + posDXI[l])/2;
                centroV[cont] = (posDY[k] + posDYI[l])/2;
                out->dados[0][centroV[cont]][centroH[cont]] = 1.0f;
                cont++;
            }
        }
    }

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
