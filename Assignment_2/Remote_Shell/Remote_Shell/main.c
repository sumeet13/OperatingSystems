/****************** CLIENT CODE ****************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 2048
#define MAX 2048
int error(char *msg){
    perror(msg);
    exit(0);
}
int main(int argc, char *argv[]){
    while(1){
        int clientSocket, port_number,n;
        char buffer[BUFFER_SIZE];
        struct sockaddr_in serverAddr;
        struct hostent *server;
        socklen_t addr_size;
        
        /*---- Create the socket. The three arguments are: ----*/
        /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
        port_number = atoi(argv[2]);
        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        
        if (clientSocket<0){
            error("Error opening clients socket\n");
        }
        
        server = gethostbyname(argv[1]);
        if (server==NULL){
            fprintf(stderr,"No such host \n\n");
            exit(0);
        }
        
        bzero((char *) &serverAddr, sizeof(serverAddr));
        /*---- Configure settings of the server address struct ----*/
        /* Address family = Internet */
        serverAddr.sin_family = AF_INET;
        
        /* Set port number, using htons function to use proper byte order */
        serverAddr.sin_port = htons(port_number);
        
        /* Set IP address to localhost */
        
        bcopy((char *)server->h_addr,(char *)&serverAddr.sin_addr.s_addr,server->h_length);
        /* Set all bits of the padding field to 0 */
        memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
        
        /*---- Connect the socket to the server using the address struct ----*/
        addr_size = sizeof(serverAddr);
        
        if (connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size)<0){
            error("Error connecting client to server\n");
        }
        
        printf("command please> ");
        bzero(buffer,BUFFER_SIZE);
        fgets(buffer,BUFFER_SIZE,stdin);

        
        n = write(clientSocket,buffer,strlen(buffer));
        if (n < 0)
        {
            error("ERROR writing to socket\n");
        }
        bzero(buffer,BUFFER_SIZE);
       
        n = read(clientSocket,buffer,BUFFER_SIZE);
        if (n < 0)
        {
            error("ERROR reading from socket\n");
        }
        
        printf("%s\n",buffer);
        
        if (strcmp(buffer,"quit\n")==0){
            printf("Server exited successfully\n");
            exit(0);
            break;
        }
        close(clientSocket);
    }
    return 0;
}
