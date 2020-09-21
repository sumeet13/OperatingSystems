//
//  main.c
//  Cat
//
//  Created by Sumeet Maan on 9/3/20.
//  Copyright © 2020 Sumeet Maan. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, const char *argv[]) {
    // insert code here...
    char *filename;
    char c;
    int size = (int)strlen(argv[1]);
    filename =malloc (sizeof(char) * size);
    strcpy(filename, argv[1]);
    
    
    FILE *ifp;
    
    ifp = fopen(filename, "r");

    if (ifp == NULL) {
      fprintf(stderr, "Can't open input file in list!\n");
      exit(1);
    }
    
    c = fgetc(ifp);
    while (c != EOF)
    {
        printf ("%c", c);
        c = fgetc(ifp);
    }
    fclose(ifp);
    return 0;
}

