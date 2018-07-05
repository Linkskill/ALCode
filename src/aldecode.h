#ifndef __ALDECODE_H
#define __ALDECODE_H

#include "pdi.h"
#include "alcode.h"

Imagem *restringeCentro(Imagem *in);
Imagem *atribuiImagemCinza(Imagem *in, float limiar);
Imagem *restringeFloodFill (Imagem *centro);
Imagem *rotaciona(Imagem *in);
void decodifica(Imagem *in, int posX, int posY, int passo);

#endif