encode:
	gcc -o al_encode main.c imagem.c alcode.c base.c cores.c desenho.c filtros2d.c geometria.c segmenta.c -Wall -Wextra -lm

decode:
	gcc -o al_decode aldecode.c imagem.c alcode.c base.c cores.c desenho.c filtros2d.c geometria.c segmenta.c -Wall -Wextra -lm

debug:
	gcc -g aldecode.c imagem.c alcode.c base.c cores.c desenho.c filtros2d.c geometria.c segmenta.c -Wall -Wextra -lm

clean:
	rm al_encode al_decode a.out
	rm ../resultados/*
