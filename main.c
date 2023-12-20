#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory_setup.h"

int main(int argc, char **argv){
    switch (argc) {
        case 1:
            fprintf(stderr, "Please input a file name\n");
            return 1;
        case 2:
            break;
        default:
            fprintf(stderr, "Please use the correct number of args\n");
            return 1;
    }
    FILE *cart = fopen(argv[1], "rb");
    if(cart == NULL){
        fprintf(stderr, "File does not exist\n");
        fclose(cart);
        return 1;
    }
    int mb = allocate_memory_banks(cart);
    if(mb == 1){
        return 1;
    }
    mb = copy_rom(cart);
    if(mb == 1){
        return 1;
    }

    fclose(cart);
}