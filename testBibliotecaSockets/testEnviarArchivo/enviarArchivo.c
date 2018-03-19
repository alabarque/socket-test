/*
 ============================================================================
 Name        : testArchivo.c
 Author      : lalala
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "../sockets/biblio_sockets.c"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {

	int socketEscucha;

	char* ipAConectar = "192.168.1.38";
	int puertoAConectar = 5000;
	char* rutaArchivo = "/home/utnso/workspace/BibliotecaSockets/nombres.csv";

	int contadorfs = 1;
	printf("intentado conectarse al puerto %d\n", puertoAConectar);
	socketEscucha = conectarseA(ipAConectar, puertoAConectar);
	while(socketEscucha == 0){
		sleep(contadorfs++);
		socketEscucha = conectarseA(ipAConectar, puertoAConectar);
		perror("no se encuentra el servidor\n");
	}

	printf("id de socket %d\n", socketEscucha);


	/* CARGAR ARCHIVO  (copypaste de internet) */

	void *miArchivo = NULL;
	FILE *archivo = fopen(rutaArchivo, "r");
	if (archivo != NULL) {
		/* Go to the end of the file. */
		if (fseek(archivo, 0L, SEEK_END) == 0) {
			/* Get the size of the file. */
			long bufsize = ftell(archivo);
			if (bufsize == -1) { /* Error */ }

			/* Allocate our buffer to that size. */
			miArchivo = malloc(sizeof(char) * (bufsize + 1));

			/* Go back to the start of the file. */
			if (fseek(archivo, 0L, SEEK_SET) != 0) { /* Error */ }

			/* Read the entire file into memory. */
			size_t newLen = fread(miArchivo, sizeof(char), bufsize, archivo);
			printf("tamanio archivo (y bytes a enviar) %d\n", newLen);
			printf("tamanio strlen %d\n", strlen(miArchivo));

			if ( ferror( archivo ) != 0 ) {
				fputs("Error reading file", stderr);
			} else {
			 char* buffer =	(char*) miArchivo; /* Just to be safe. */
			 buffer[newLen] = '\0';
			}
		}
	}

	/* FIN CARGA ARCHIVO */


	/* TEST CONTINGENCIA: se graba el archivo para chequear que se cargo bien en memoria  */

	FILE* archivoContingencia;
			char* rutaArchivoContingencia = "/tmp/testRecibidoContingencia";

			archivoContingencia = fopen(rutaArchivoContingencia, "w");

			if(fwrite(miArchivo, sizeof(char), strlen(miArchivo), archivoContingencia) != strlen(miArchivo)){
				perror("fallo al escribir el buffer en el archivo\n");
				exit(1);
	}

	fclose(archivoContingencia);

	/* ENVIO DE DATOS */
	int bytesEnviados = 0;

	while(bytesEnviados < strlen(miArchivo)){

	bytesEnviados += enviarMensajeArchivo(socketEscucha, (char *) (miArchivo + bytesEnviados));

	}

	printf("bytes Enviados: %d\n", bytesEnviados);


	fclose(archivo);
	close(socketEscucha);


	/* MD5 ARCHIVO POSTA */

	char* md5 = malloc(100);
	strcpy(md5,"md5sum ");
	strcat(md5, rutaArchivo);
	printf("md5 del archivo original: \n");
	system(md5);

	/* MD5 ARCHIVO CONTINGENCIA */

	char* md5Contingencia = malloc(100);
	strcpy(md5Contingencia,"md5sum ");
	strcat(md5Contingencia, rutaArchivoContingencia);
	printf("md5 del archivo de contingencia: \n");
	system(md5Contingencia);


	free(md5);
	free(md5Contingencia);
//	free(rutaArchivo);
//	free(rutaArchivoContingencia);
//	free(ipAConectar);


	puts("fin del programa \n"); /* está para chequear que llego al final */
	return EXIT_SUCCESS;
}
