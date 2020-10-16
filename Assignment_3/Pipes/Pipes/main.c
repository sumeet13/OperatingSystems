#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define MSGSIZE 16
#define READ_END 0
#define WRITE_END 1

void convertToUpperCase(char *s){
    for(int i=0; s[i]!= '\0';i++){
        if( s[i]>='a' && s[i]<='z'){
            s[i] = s[i]-32;
        }
    }
}
void leftRotateByHalf(char *s){
    int n = strlen(s);
    int pivot = n/2;
    if (n != 0) { //if 0, don't rotate
        reverse(s, 0, pivot);
        reverse(s, pivot, n);
        reverse(s, 0, n);
    }
}
int main( )
{
    char *msg1 = " How are you?";
    char inbuff[MSGSIZE];
    int p[2], pid;
    pipe(p);
    pid = fork();
    
    if (pid<0){
        fprintf(stderr, "Could not perform first fork");
    }
    if (pid == 0){
        //child process
        close(p[READ_END]);
        convertToUpperCase(msg1);
        write(p[1],msg1,MSGSIZE);
        close(p[WRITE_END]);
    }
    else
    {
        //parent process
        int fd[2], c_pid;
        pipe(fd);
        char inbuff2[MSGSIZE];
        c_pid = fork();
        if (c_pid<0){
            fprintf(stderr, "Could not perform second fork ");
        }
        if (c_pid == 0){
            close(p[WRITE_END]);
            read(p[READ_END], inbuff, MSGSIZE);
            printf("The message in Upper case is %s",inbuff);
            close(p[READ_END]);
            leftRotateByHalf(inbuff);
            
            close(fd[READ_END]);
            write(fd[WRITE_END],inbuff,MSGSIZE);
            close(fd[WRITE_END]);
        }
        else{
            if(pid==0){
                close(fd[WRITE_END]);
                read(fd[READ_END], inbuff2, MSGSIZE);
                printf("Rotated String: %s",inbuff2);
                close(fd[READ_END]);
            }
            wait(NULL);
        }
        wait(NULL);
    }
    exit(0);
}
