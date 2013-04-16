/* 
 * File:   telnetclient.c
 * Author: ruben
 *
 * Created on 25 de febrero de 2013, 12:00
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/*Variables globales*/
int socket_fd;
int output_fd;

/*
 * 
 */
int main(int argc, char** argv) {
    
    struct hostent *hp;
    struct sockaddr_in srv;
    char *ip = argv[1];
    int puerto = atoi(argv[2]);
    
    if (argc < 3){
        printf("Numero de argumentos no valido");
        return (-1);
    }

    /*Creando conexion al servidor*/
    srv.sin_family = AF_INET;
    srv.sin_port = htons(puerto);
    srv.sin_addr.s_addr = inet_addr(argv[1]);
    
    /*Creando el socket*/
    if((socket_fd=socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket");
        exit(1);
    }
    
    /*Conexion al servidor*/
    
    printf("Conectando a %s:%d... \n", ip, puerto);
    if (connect(socket_fd, (struct sockaddr*) &srv, sizeof(srv))<0){
        perror("connect");
        exit(1);
    } else {
        printf("Conexion establecida\n");
    }
    
    /*Leyendo el socket*/
    
    char buf[1000],mensaje[100];
    //buf = malloc((1000)*sizeof(char));
    int numbytes;
    int i;
    fflush(stdin);
    while(strcmp(mensaje,"exit")!=0){
        numbytes=0;
        
        for(i=0;i<100;i++){
            mensaje[i]='\0';
        }
        
        printf("> ");
        fflush(stdin);
        gets(mensaje);
        scanf("%s", mensaje);
        
        /*Enviar mensaje*/
        if(send(socket_fd,mensaje, strlen(mensaje),0)<0){
            puts("Envio fallido");
            break;
        }
        
        
        /*Recibir mensaje*/
        if ((numbytes=recv(socket_fd,buf,100-1,0)) == -1){
            printf("Error en recv \n");
            exit(-1);
        }
        buf[numbytes]='\0';
        if(numbytes>0){
            printf("numbytes: %d\n",numbytes);
            //buf[numbytes]='\0';
            
            puts("Respuesta: ");
            //puts(buf);
            printf("%s\n",buf);
        }
        
        memset(&buf[0],0,sizeof(buf));
        /*for(i=0;i<2000;i++){
            buf[i]='\0';
        }*/
        //fflush(stdout);
       
        
    }
        
    close(socket_fd);
    return (EXIT_SUCCESS);
}

