#ifndef FILE_FUNCIONES_SEEN
#define FILE_FUNCIONES_SEEN
#include "cliente.h"
#endif

/*Funciones que manejan la conexion*/
#ifndef FILE_SOCKETS_SEEN
#define FILE_SOCKETS_SEEN
#include "sockets.h"
#endif

#define ARGC 3
#define OPTION 0
#define REALM 1
#define KEY 2

int main(int argc, char const *argv[])
{
	printf("You are running the client...\n");

	/*Variables*/
	struct sockaddr_in client; //Declaración de la estructura con información para la conexión
	struct hostent *server;	//Declaración de la estructura con información del host
	int server_socket, fd, read_bytes, write_bytes;
	char *buffer, option[2];
	int i, tam, size;
	short orden[ARGC];
	FILE *file;
	/*Comprobar cantidad correcta de argumentos*/
	if (argc != ARGC * 2 + 1)
	{
		fprintf(stdout, "Use: -o <option> -r <realm_address> -k <key_file>\n");
		fprintf(stdout, "Use: -o <option> -r <realm_address> -n <key_name>");
		exit(10);
	}

	/*Ordenar los argumentos*/
	for (i = 1; i < 2 * ARGC; i++)
	{
		if (!strcmp(argv[i], "-o"))
			orden[OPTION] = i + 1;
		else if (!strcmp(argv[i], "-r"))
			orden[REALM] = i + 1;
		else if (!strcmp(argv[i], "-k") || !strcmp(argv[i], "-n"))
			orden[KEY] = i + 1;
		else
		{
			printf("%s is not a valid option\n", argv[i]);
			exit(5);
		}
		i++;
	}
	if (!strcmp(argv[orden[REALM]], "127.0.0.1"))
	{
		printf("Debes escribir la IP en la red\n");
		return -1;
	}
	if (strcmp(argv[orden[OPTION]], "start") && strcmp(argv[orden[OPTION]], "enter"))
	{
		/* Abrir el archivo que contiene la clave */
		if ((file = fopen(argv[orden[KEY]], "r")) == NULL)
		{
			printf("El archivo \"%s\" no existe\n", argv[orden[KEY]]);
			exit(-1);
		}
		if (fseek(file, 0L, SEEK_END) < 0)
			fail_err();
		size = ftell(file);
		tam = size + strlen(argv[orden[REALM]]) + strlen(argv[orden[OPTION]]) + 2;
		rewind(file);
		if ((buffer = (char *)malloc(tam * sizeof(char))) == NULL)
			fail_err();
		strcpy(buffer, argv[orden[OPTION]]);
		strcat(buffer, " ");
		strcat(buffer, argv[orden[REALM]]);
		strcat(buffer, " ");
		fread(buffer + strlen(buffer), sizeof(char), size, file);
		fclose(file);
	}
	else
	{
		tam = strlen(argv[orden[KEY]]) + strlen(argv[orden[REALM]]) + strlen(argv[orden[OPTION]]) + 2;
		if ((buffer = (char *)malloc(tam * sizeof(char))) == NULL)
			fail_err();
		strcpy(buffer, argv[orden[OPTION]]);
		strcat(buffer, " ");
		strcat(buffer, argv[orden[REALM]]);
		strcat(buffer, " ");
		strcat(buffer, argv[orden[KEY]]);
	}
	/*Inicializar*/
	if (strcmp(argv[orden[OPTION]], "add") && strcmp(argv[orden[OPTION]], "start"))
		server = gethostbyname(argv[orden[REALM]]);
	else
		server = gethostbyname("127.0.0.1");
	bzero((char *)&client, sizeof((char *)&client));

	client.sin_family = AF_INET;
	client.sin_port = htons(PORT);
	bcopy((char *)server->h_addr, (char *)&client.sin_addr.s_addr,
		  sizeof(server->h_length));
	/*Crear server_socket*/
	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		fail_err();
	printf("Socket...\n");

	/*Hacer socket*/
	if (connect(server_socket, (struct sockaddr *)&client,
				sizeof(client)) < 0)
		fail_err();
	printf("Connect...\n");

	printf("Connecting with %s:%d\n", inet_ntoa(client.sin_addr),
		   htons(client.sin_port));

	/* Enviar opcion e ip */
	if ((write_bytes = write(server_socket, buffer, tam)) < 0)
		fail_err();

	if (!strcmp(argv[orden[OPTION]], "sign"))
	{
		char *file = (char *)malloc((sizeof(argv[orden[KEY]]) + 5) * sizeof(char));
		strcpy(file, "king-");
		strcat(file, argv[orden[KEY]]);
		send_rcv_file(file, server_socket);
	}
	else if (!strcmp(argv[orden[OPTION]], "enter"))
	{
		char *file = (char *)malloc(30 * sizeof(char));
		strcpy(file, "../KING-SERVER/table");
		send_rcv_file(file, server_socket);
	}
	else
	{
		send_ask_add(server_socket);
	}
	free(buffer);
	close(server_socket);
	return 0;
}
