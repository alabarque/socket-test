/*
 * testRecibirArchivo.c
 *
 *  Created on: 19/3/2018
 *      Author: utnso
 */


#include "../sockets/biblio_sockets.c"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void){

	int puerto = 5000;
	int socket;

	printf("escuchando el puerto %d\n", puerto);
	if((socket = escuchar(puerto)) != -1){
		int nuevoSocket;
		fd_set* readSet = malloc(sizeof(fd_set));
		FD_ZERO(readSet);
		FD_SET(socket, readSet);
		printf("esperando conexiones\n");
		nuevoSocket = esperarConexionesSocket(readSet, socket);

		if(nuevoSocket != -1){

			char* miArchivo = recibirMensajeArchivo(nuevoSocket);

			FILE* archivoRecibido;
			char* rutaArchivoRecibido = "/tmp/testArchivoRecibido";

			archivoRecibido = fopen(rutaArchivoRecibido, "w");

			if(fwrite(miArchivo, sizeof(char), strlen(miArchivo), archivoRecibido) != strlen(miArchivo)){
				perror("fallo al escribir el buffer en el archivo\n");
				exit(1);
			}

			fclose(archivoRecibido);


			char* md5 = malloc(100);
			strcpy(md5,"md5sum ");
			strcat(md5, rutaArchivoRecibido);
			printf("md5 del archivo recibido: \n");
			system(md5);

			free(md5);

		}

		close(socket);

	}

	printf("finalizacion exitosa \n");
	return EXIT_SUCCESS;
}
