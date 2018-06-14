encode:
	gcc main.c imagem.c alcode.c base.c cores.c desenho.c filtros2d.c geometria.c segmenta.c -Wall -Wextra -lm
	./a.out

decode:
	gcc aldecode.c imagem.c alcode.c base.c cores.c desenho.c filtros2d.c geometria.c segmenta.c -Wall -Wextra -lm
	./a.out

debug:
	gcc -g *.c -Wall -Wextra -lm

clean:
	rm a.out
	rm ../resultados/*
