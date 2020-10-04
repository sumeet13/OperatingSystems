#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
//#include <fcntl.h>

#define BUFFER_SIZE 2048
#define MAX 2048

void error(char *msg){
    perror(msg);
    exit(1);
}

void parsing(char *inputBuffer, char **args){
    char *s = " \n";
    char *token;
    int i = 0;
    
    token = strtok(inputBuffer, s);
    
    while( token != NULL){
        args[i] = token;
        i++;
        token = strtok(NULL,s);
    }
    args[i] = NULL;
    free(token);
    
}
int main(int argc, char *argv[]){
    int welcomeSocket, newSocket, port_number, client_len;
    struct sockaddr_in serverAddr, clientAddr;
    int n;
    
    if (argc<2){
        fprintf(stderr,"No port number provided\n");
        exit(1);
    }
    /*---- Create the socket. The three arguments are: ----*/
    /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
    welcomeSocket = socket(AF_INET, SOCK_STREAM, 0);
    
    if (welcomeSocket<0){
        error("Error while opening socket\n");
    }
    /*---- Clear serverAddress before using serverAddr ----*/
    bzero((char *) &serverAddr, sizeof(serverAddr));
    port_number = atoi(argv[1]);
    
    /*---- Configure settings of the server address struct ----*/
    /* Address family = Internet */
    serverAddr.sin_family = AF_INET;
    /* Set port number, using htons function to use proper byte order */
    serverAddr.sin_port = htons(port_number);
    /* Set IP address to localhost */
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    /* Set all bits of the padding field to 0 */
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
    
    /*---- Bind the address struct to the socket ----*/
    if (bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr))<0){
        error("Error on Binding\n");
    }
    
    /*---- Listen on the socket, with 5 max connection requests queued ----*/
    if(listen(welcomeSocket,5)==0)
        printf("Listening\n");
    else
        printf("Error\n");
    
    client_len = sizeof(clientAddr);
    
    /*---- Accept call creates a new socket for the incoming connection ----*/
    while(1){
        
        newSocket = accept(welcomeSocket, (struct sockaddr *) &clientAddr, &client_len);
        
        if (newSocket<0){
            error("Error while accepting\n");
            break;
        }
        printf("Connection established with %s port %d\n",  inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
        
        fflush(NULL);
        pid_t pidC = fork();
        
        if(pidC == 0){
            
            char *buffer = malloc(sizeof(char) * MAX);
            char **args  = malloc(sizeof(char *) * 6); //max number of parameters accepted
            dup2(newSocket, STDOUT_FILENO);
            dup2(newSocket, STDERR_FILENO);
            
            while(1){
                n = read(newSocket,buffer,MAX);
                if (n<0){
                    perror("Error while reading\n");
                    break;
                }
                if(strncmp(buffer, "quit", 4)==0){
                    printf("quit");
                    close(newSocket);
                    break;
                }
                /******************************/
                parsing(buffer,args);
                
                pid_t pidH = fork();
                if (pidH > 0){        //parent process
                    wait(NULL);
                    //waitpid(pidH, NULL, 0);
                }
                else if (pidH == 0){  //child process
                    
                    if (execvp(args[0],args)< 0){
                        printf("Could not execvp command\n");
                    }
                }
                else{                 //error
                    error("Error while forking\n");
                }
                /********************************/
                //clear buffer and args for next reads
                memset(buffer,'\0', sizeof(char) * MAX);
                memset(args,'\0', sizeof(char *) * 6);
            }
        }
        else if (pidC < 0){
            printf("Error while fork");
            exit(1);
        }
    }
    close(welcomeSocket);
    return 0;
}
