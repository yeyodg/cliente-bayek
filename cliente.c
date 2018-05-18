#ifndef FILE_CLIENTE_SEEN
#define FILE_CLIENTE_SEEN
#include "cliente.h"
#endif

#ifndef FILE_SOCKETS_SEEN
#define FILE_SOCKETS_SEEN
#include "sockets.h"
#endif

char buffer[1024];

int send_rcv_file(char *file, int server_socket)
{
	char buffer[BUFFER_SIZE];
	int read_bytes, fd;
	read_bytes = 0;
	if ((fd = open(file, O_CREAT | O_WRONLY, S_IRWXU)) < 0)
		fail_err();
	memset(buffer, 0, BUFFER_SIZE);
	do
	{
		read_bytes = read(server_socket, buffer, BUFFER_SIZE);
		if (!strcmp(buffer, "Error: Clave no valida\n") || !strcmp(buffer, "Error: No valido\n") ||
			!strcmp(buffer, "Error: Clave no importada\n"))
		{
			close(fd);
			unlink(file);
			free(file);
			printf("%s", buffer);
			return -1;
		}
		else if (write(fd, buffer, read_bytes) < 0)
			fail_err();
	} while (read_bytes == BUFFER_SIZE);
	/* Cerrar los file descriptors */
	close(fd);
	printf("Archivo recibido\n");
	free(file);
	return 0;
}

int send_ask_add(int server_socket)
{
	char buffer[BUFFER_SIZE];
	int read_bytes;
	memset(buffer, 0, BUFFER_SIZE);
	do
	{
		read_bytes = read(server_socket, buffer, BUFFER_SIZE);
		buffer[read_bytes] = '\0';
		printf("%s", buffer);
	} while (read_bytes == BUFFER_SIZE);
	return 0;
}