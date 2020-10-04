/****************** CLIENT CODE ****************/
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 2048
#define MAX 50

int error(char *msg){
    perror(msg);
    exit(0);
}

int main (int argc, char *argv[]){
    int clientSocket, port_number,n;
    struct sockaddr_in serverAddr;
    struct hostent *server;
    socklen_t addr_size;
    
    char *readBuffer = malloc(sizeof(char) * BUFFER_SIZE);
    char *userBuffer = malloc(sizeof(char) * MAX);
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
        exit(1);
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
    
    while(1){
        //clear buffers for read user line and receive server output
        memset(userBuffer, '\0', sizeof(char) * MAX);
        memset(readBuffer, '\0', sizeof(char) * BUFFER_SIZE);
        
        printf("command please> ");
        
        fgets(userBuffer,sizeof(char) * MAX , stdin);
        printf("### %s", userBuffer);
        
        //sending command to server
        n = write(clientSocket,userBuffer,strlen(userBuffer));
        if (n < 0){
            error("ERROR writing to socket\n");
        }
        
        sleep(1);
        //reading the output from server
        n = read(clientSocket,readBuffer,sizeof(char)*BUFFER_SIZE);
        if (n < 0){
            error("ERROR reading from socket\n");
        }
        else{
            printf(" %s", readBuffer);
        }
        
        //compare buffer to quit
        if (strncmp(userBuffer, "quit", 4)==0){
            printf("Client Disconnected\n");
            close(clientSocket);
            exit(0);
            break;
        }
    }
    //close(clientSocket);
    return 0;
}
