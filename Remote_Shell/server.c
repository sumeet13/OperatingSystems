#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>


#define BUFFER_SIZE 2048
#define MAX 2048
void error(char *msg){
    perror(msg);
    exit(1);
}

void parsing(char inputBuffer[], char *args[])
{
    const char s[4] = " \t\n";
    char *token;
    token = strtok(inputBuffer, s);
    int i = 0;
    while( token != NULL)
    {
        args[i] = token;
        i++;
        //printf("%s\n", token);
        token = strtok(NULL,s);
    }
    args[i] = NULL;
    free(token);
    
}
int main(int argc, char *argv[]){
    int welcomeSocket, newSocket, port_number, client_len;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in serverAddr, clientAddr;
    char data[MAX];
    data[0] = '\0';
    memset(data,0,MAX);
    int n;
    char *args[100];
    
    
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

//        dup2(newSocket, STDERR_FILENO);
        if (newSocket<0){
            error("Error while accepting\n");
        }
        
        
        
        bzero(buffer, BUFFER_SIZE);
        /*---- Send message to the socket of the incoming connection ----*/
        n = read(newSocket,buffer,BUFFER_SIZE);
        if (n<0){
            error("Error while reading\n");
        }
        

       
        strcpy(data,  buffer);
        
        if (strcmp(data, "quit\n")==0){
            
            
            send(newSocket,data,20,0);
            
            exit(0);
            break;
        } // this will force the code to exit
        
        parsing(data,args);
        
        pid_t pid = fork();
        if (pid >0){//parent process
            
            wait(NULL);
            send(newSocket,"Success",20,0);
            
        }
        
        else if (pid==0){//child process
            dup2(newSocket, STDOUT_FILENO);
            if (execvp(args[0],args)< 0){
                error("Could not execvp command\n");
            }
           
            
        }
        else{
            //pid<0
            error("Error while forking\n");
            exit(1);
        }
    }
  
    close(newSocket);
    close(welcomeSocket);
    exit(0);
    
    return 0;
    
    
    
}
