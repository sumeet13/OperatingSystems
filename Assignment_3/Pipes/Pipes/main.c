#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MSGSIZE 16
#define READ_END 0
#define WRITE_END 1
int main( )
{
    char *msg1 = " How are you?";
    char inbuff[MSGSIZE];
    int p[2], pid;
    pipe(p);
    pid = fork();
    
    if (pid<0){
        fprintf(stderr, "Could not fork");
    }
    if (pid == 0){
        //child process
        close(p[READ_END]);
        
        write(p[1],msg1,MSGSIZE);
        close(p[WRITE_END]);
    }
    else
    {
       //parent process
        
    }
    exit(0);
}
