#ifndef __ALDECODE_H
#define __ALDECODE_H

#include "pdi.h"
#include "alcode.h"

Imagem *restringeCentro(Imagem *in);
Imagem *atribuiImagemCinza(Imagem *in);
Imagem *restringeFloodFill (Imagem *in);
Imagem *rotaciona(Imagem *in);
void decodifica(Imagem *in);

#endif