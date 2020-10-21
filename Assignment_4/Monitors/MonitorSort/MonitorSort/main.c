//
//  main.c
//  SemMonitors
//
//  Created by Sumeet Maan on 10/13/20.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
int ** matrix;
int n, counter;
int phase = 1;
bool *status;
char *filename;
pthread_mutex_t *mutex;
pthread_cond_t *cond;
/*----------------------------Function to print the matrix -------------------------------- */
void printMatrix() {
    printf("\n");
    for (int i = 0; i < n; i++) {
        for (int count = 0; count < n; count++) {
            
            printf(" %d", matrix[i][count]);
        }
        printf("\n");
        
    }
}

/*----------------------------Function to read the input file ------------------------------ */
void readInputFromFile() {
    FILE * fp;
    matrix = malloc(n * sizeof( * matrix));
    
    int count = 1;
    for (int i = 0; i < n; i++) {
        matrix[i] = malloc(count * sizeof( ** matrix));
        if (!matrix[i]) /* If `malloc` failed */ {
            for (int j = 0; j < i; j++) /* free previously allocated memory */ {
                free(matrix[j]);
            }
            free(matrix);
            
            fputs(stderr, "Error allocating memory; Exiting code with error code");
            exit(-1);
        }
        count++;
    }
    
    /* read the input file and store it in the array*/
    //    char * filename = "/Users/smaan/Documents/Classes/OS/Assignment_4/input.txt";
    
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error while opening file");
        
    }
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fscanf(fp, "%d", &matrix[i][j]);
        }
        
    }
    
    printMatrix();
    fclose(fp);
    
}
/*----------------Function to swap(Used during insertion sort) ------------------------------ */

void swap(int * a, int * b) {
    int temp = * a;
    * a = * b;
    * b = temp;
}

/*------------Function for ascending column wise insertion sort -------------------------- */
void insertion_sort_aesc_column(int col) {
    
    int i, j, k;
    for (i = 1; i < n; ++i) {
        k = matrix[i][col];
        j = i - 1;
        while ((j >= 0) && (k < matrix[j][col])) {
            matrix[j + 1][col] = matrix[j][col];
            --j;
        }
        matrix[j + 1][col] = k;
    }
    
    
}
/*------------Function for ascending row wise insertion sort -------------------------- */
void insertion_sort_aesc_row(int row) {
    
    int i, j, k;
    for (i = 1; i < n; ++i) {
        k = matrix[row][i];
        j = i - 1;
        while ((j >= 0) && (k < matrix[row][j])) {
            matrix[row][j + 1] = matrix[row][j];
            --j;
        }
        matrix[row][j + 1] = k;
    }
    
    
}
/*------------Function for descending row wise insertion sort -------------------------- */
void insertion_sort_desc_row(int row) {
    
    int i, j, k;
    for (i = 1; i < n; ++i) {
        k = matrix[row][i];
        j = i - 1;
        while ((j >= 0) && (k > matrix[row][j])) {
            matrix[row][j + 1] = matrix[row][j];
            --j;
        }
        matrix[row][j + 1] = k;
    }
    
    
}

/*--------------------------------Master Mapping Function---------------------------------- */
void *sort(void *tid){
    
    int threadNumber = *(int *)tid;
    //printf("I am in the sorting method \n");
    
    /* For odd phase sort the rows alternatively*/
    while(1){
        
        //        if(status[threadNumber]){
        status[threadNumber] = true;
        
        pthread_mutex_lock(&mutex[threadNumber]);
        
        if(phase%2==1){
            if(threadNumber%2==0){
                insertion_sort_aesc_row(threadNumber);
            }
            else{
                insertion_sort_desc_row(threadNumber);
            }
        }
        /* For even phase sort the columns alternatively*/
        else{
            insertion_sort_aesc_column(threadNumber);
        }
        counter++;
        while(status[threadNumber])
            pthread_cond_wait(&cond[threadNumber], &mutex[threadNumber]);
        pthread_mutex_unlock(&mutex[threadNumber]);
        
        
        //        }
    }
}


/*----------------------------Main Function ------------------------------ */

int main(int argc, const char * argv[]) {
    printf("Before");
    int size = (int)strlen(argv[1]);
    filename =malloc (sizeof(char) * size);
    strcpy(filename, argv[1]);
    
    n = atoi(argv[2]);
    printf("After");
    //    n = 4;
    readInputFromFile();
    status = malloc(n * sizeof( bool));
    
    for (int i =0;i<n; i++){
        status[i]= false;
    }
    
    
    
    /* Create n threads*/
    pthread_t p[n];
    cond = (malloc(n*sizeof(pthread_cond_t)));
    mutex = (malloc(n*sizeof(pthread_mutex_t)));
    int tid[n];
    /* Initilaze the n condition variables*/
    for (int i = 0; i < n; i++) {
        cond[i]= (pthread_cond_t)PTHREAD_COND_INITIALIZER;
        mutex[i] = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    }
    
    /*Initialize and create n threads*/
    for (int i = 0; i < n; i++) {
        tid[i] = i;
        //create the threads and sort them accordingly
        pthread_create( &p[i], NULL, sort, &tid[i]);
    }

    
    
    /* Working on the phases */
    while(phase <= n + 1) {
        
        if(counter==n){
            printf("Phase %d \n", phase);
            printMatrix();
            phase++;
            counter =0;
            
            for(int i =0;i<n;i++){
                
                status[i] = false;
                pthread_cond_signal(&cond[i]);
                
            }
        }
    }
    
    /* Destroy the semaphores */
    for (int i=0; i<n; i++){
        pthread_cancel(p[i]);
        pthread_cond_destroy(&cond[i]);
        pthread_mutex_destroy(&mutex[i]);
    }
    
    return 0;
    
}
