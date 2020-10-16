//
//  main.c
//  SemSort
//
//  Created by Sumeet Maan on 10/13/20.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "InsertionSort.h"

int** matrix;

int main(int argc, const char * argv[]) {
    // insert code here...
    FILE *fp;
    
    
    int n = 4;
    matrix = malloc(n * sizeof(*matrix));
    
    
    int count = 1;
    for(int i = 0; i < n; i++)
    {
        matrix[i] = malloc(count * sizeof(**matrix));
        if(!matrix[i]) /* If `malloc` failed */
        {
            for(int j = 0; j < i; j++) /* free previously allocated memory */
            {
                free(matrix[j]);
            }
            free(matrix);
            
            fputs(stderr, "Error allocating memory; Exiting code with error code");
            exit(-1);
        }
        count++;
    }
    
    
    /* read the input file and store it in the array*/
    char* filename = "/Users/smaan/Documents/Classes/OS/Assignment_4/input.txt";
    
    fp = fopen(filename,"r");
    if(fp==NULL){
        printf("Error while opening file");
        return 1;
    }
    
    for(int i = 0; i < n; i++)
    {
        for(count = 0; count < n; count++)
        {
            fscanf(fp, "%d", &matrix[i][count]);
        }
        
    }
    
    printMatrix(matrix,n);
    fclose(fp);
    
    /* Working on the phases */
    
    for(int phase=0; phase<=n+1; phase++){
        
        
        if(phase%2==1){
            /* If the phase is odd: do the alternate row sorting */
            
            
            for(int i=0; i<n; i++){
                if(i%2==0){
                    /* Sort the row in ascending order*/
                    
                    insertion_sort_aesc_row(*(matrix+i),n,i);
                }
                else{
                    insertion_sort_desc_row(*(matrix+i),n,i);
                }
            }
            
        }
        else {
            /* If the phase is even: do the column sorting */
            for(int i=0; i<n; i++){
                
                insertion_sort_aesc_column(**matrix,n,i);
            }
            
        }
        printf("Phase %d \n",phase);
        printMatrix(matrix, n);
        
        
    }   
    return 0;
    
    
    
}
