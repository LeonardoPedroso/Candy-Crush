#makefile
all: bubble_trouble

animacoes.o: animacoes.c animacoes.h
	gcc -c -g -D_THREAD_SAFE -I/usr/local/include/SDL2 -D_REENTRANT -Wall -pedantic -std=c99 animacoes.c
graphics.o: graphics.c graphics.h
	gcc -c -g -D_THREAD_SAFE -I/usr/local/include/SDL2 -D_REENTRANT -Wall -pedantic -std=c99  graphics.c
op_listas.o: op_listas.c op_listas.h
	gcc -c -g -Wall -pedantic -std=c99 op_listas.c
leitura_dados.o: leitura_dados.c leitura_dados.h header_base.h
    gcc -c -g -Wall -pedantic -std=c99 leitura_dados.c
resultados.o: resultados.c resultados.h
    gcc -c -g -Wall -pedantic -std=c99 resultados.c
main_loop.o: main_loop.c op_listas.h leitura_dados.h graphics.h animacoes.h resultados.h
	gcc -c -g -D_THREAD_SAFE -I/usr/local/include/SDL2  -D_REENTRANT -Wall -pedantic -std=c99 main_loop.c
bubble_trouble: main_loop.o graphics.o leitura_dados.o resultados.o animacoes.o op_listas.o
	gcc -g -D_THREAD_SAFE -I/usr/local/include/SDL2 -L/usr/local/lib -D_REENTRANT -lm -std=c99 -lSDL2 -lSDL2_gfx main_loop.o graphics.o animacoes.o leitura_dados.o resultados.o op_listas.o -o bubble_trouble

clean:
	rm main_loop.o op_listas.o graphics.o leitura_dados.o resultados.o animacoes.o bubble_trouble
run: 
	./bubble_trouble -f -l 13 -a 13 -b 45