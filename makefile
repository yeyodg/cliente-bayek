#Borrar todos los .o con $make clean

main: main.o cliente.o
	gcc -g -w -o  bayek main.o cliente.o `gpgme-config --cflags --libs`

main.o: main.c cliente.c sockets.h
	gcc -c -g -w main.c

cliente.o: cliente.c cliente.h
	gcc -c -g -w cliente.c

clean: 
	rm main.o cliente.o