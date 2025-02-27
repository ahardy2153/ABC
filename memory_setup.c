#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned short num_rom_banks;
uint8_t **mem_banks;
uint8_t *rom1_bank;
uint8_t *rom2_bank;
unsigned short num_ram_banks;
uint8_t **ram_bank;
uint8_t *cur_ram_bank;

int allocate_memory_banks(FILE *cart){
    if (fseek(cart, 327, SEEK_SET) != 0) {
        perror("Error seeking in file");
        fclose(cart);
        return 1;
    }
    unsigned char rom_size[3];
    fread(rom_size, sizeof(rom_size), 1, cart);
    printf("%02x ", rom_size[0]);
    printf("%02x ", rom_size[1]);
    printf("%02x\n", rom_size[2]);
    printf("Allocating %d ROM Banks\n", (2 << rom_size[1]));
    num_rom_banks = 2 << rom_size[1];
    mem_banks = malloc(sizeof (u_int8_t*) * (2 << rom_size[1]));
    switch (rom_size[2]) {
        case 0x00:
        case 0x01:
            printf("Allocating 0 RAM Banks\n");
            ram_bank = NULL;
            num_ram_banks = 0;
            break;
        case 0x02:
            printf("Allocating 1 RAM Banks\n");
            ram_bank = malloc(sizeof (u_int8_t));
            num_ram_banks = 1;
            break;
        case 0x03:
            printf("Allocating 4 RAM Banks\n");
            ram_bank = malloc(sizeof (u_int8_t) * 4);
            num_ram_banks = 4;
            break;
        case 0x04:
            printf("Allocating 16 RAM Banks\n");
            ram_bank = malloc(sizeof (u_int8_t) * 4);
            num_ram_banks = 16;
            break;
        case 0x05:
            printf("Allocating 8 RAM Banks\n");
            ram_bank = malloc(sizeof (u_int8_t) * 4);
            num_ram_banks = 8;
            break;
    }

    for(int i = 0; i < 2 << rom_size[1]; i++){
        mem_banks[i] = malloc(sizeof (u_int8_t) * 1024 * 16);
        if(mem_banks[i] == NULL){
            fprintf(stderr, "Creation of Memory banks failed\n");
            return 1;
        }
    }
    return 0;
}

void print_memory(){
    for (int i = 0; i < num_rom_banks; i++) {
        int k = 0;
        for(int j = 0; j < 16384; j++){
            if(k == 8){
                printf("\t");
            }
            if(k == 16){
                printf("\n");
                k = 0;
            }
            printf("%02x ", mem_banks[i][j]);
            k++;
        }
        printf("\n");
    }
}


int copy_rom(FILE *cart){
    if (fseek(cart, 0, SEEK_SET) != 0) {
        perror("Error seeking in file");
        fclose(cart);
        return 1;
    }
    unsigned char bank[16384];
    int cur = 0;
    while(fread(bank, sizeof(bank), 1, cart) != 0){
        printf("Copying bank %d\n", cur);
        memcpy(mem_banks[cur], bank, 16384);
        cur++;
    }
    printf("Verifying integrity\n");
    print_memory();
    return 0;
}


