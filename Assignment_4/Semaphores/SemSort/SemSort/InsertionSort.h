//
//  InsertionSort.h
//  SemSort
//
//  Created by Sumeet Maan on 10/13/20.
//

#ifndef InsertionSort_h
#define InsertionSort_h
void swap(int *a, int *b);
void insertion_sort_aesc_row(int *a,const int size,const int row);
void insertion_sort_desc_row(int *a,const int size, const int row);
void insertion_sort_aesc_column(int **a,const int size, const int row);
void printMatrix(int **arr, int n);
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
void insertion_sort_aesc_row(int *a,const int size,const int row)
{
 
    int i, j, k;
    for (i = 1; i < size; ++i)
    {
        k = a[i];
        j = i - 1;
        while ((j >= 0) && (k < a[j]))
        {
            a[j + 1] = a[j];
            --j;
        }
        a[j + 1] = k;
    }
}

void insertion_sort_desc_row(int *a,const int size, const int row)
{
 
    int i, j, k;
    for (i = 1; i < size; ++i)
    {
        k = a[i];
        j = i - 1;
        while ((j >= 0) && (k > a[j]))
        {
            a[j + 1] = a[j];
            --j;
        }
        a[j + 1] = k;
    }
}
void insertion_sort_aesc_column(int **a,const int size, const int col)
{
 
    int i, j, k;
    for (i = 1; i < size; ++i)
    {
        k = a[i][col];
        j = i - 1;
        while ((j >= 0) && (k < a[j][col]))
        {
            a[j + 1][col] = a[j][col];
            --j;
        }
        a[j + 1][col] = k;
    }
}

void printMatrix(int **arr, int n){
    printf("\n");
    for(int i = 0; i < n; i++)
    {
        for(int count = 0; count < n; count++)
        {
            printf(" %d", arr[i][count]);
        }
        printf("\n");
        
    }
}
#endif /* InsertionSort_h */
