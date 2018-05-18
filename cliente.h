#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// Librerias de GPG
#include <gpgme.h>
#include <locale.h>

#include <fcntl.h>

/* Not technically required, but needed on some UNIX distributions */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define NOT_IMPORTED -1
#define NOT_VALID -2
#define SIGNED 1
#define KNOWN 2
#define UNKNOWN -3
#define INVALID -4

#define fail_err()                                       \
    {                                                    \
        fprintf(stderr, "%s:%d: %s\n",                   \
                __FILE__, __LINE__, strerror(errno));    \
        exit(__LINE__);                                  \
    }

/* 
    Se encarga de manejar la respuesta del servidor cuando se usa la opción de 
    firmar una clave
    Recibe: El nombre del archivo que contiene la clave y el file descriptor
    del socket coectado con el servidor
*/
int send_rcv_file(char *file_name, int server_socket);
int send_ask_add(int server_socket);