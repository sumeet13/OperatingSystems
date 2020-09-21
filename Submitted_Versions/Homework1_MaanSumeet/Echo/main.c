//
//  main.c
//  Echo
//
//  Created by Sumeet Maan on 9/1/20.
//  Copyright © 2020 Sumeet Maan. All rights reserved.
//

#include <stdio.h>

int main(int argc, const char * argv[]) {
    // insert code here...
    int counter;
    for(counter =0; counter<argc; counter++)
    {
        printf("arg[%d]: %s \n ", counter, argv[counter]);
    }
    return 0;
}
