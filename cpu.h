#ifndef CPU_H
#define CPU_H
#include <stdint.h>

uint8_t get_PC();

void set_register(uint8_t register_dest, uint8_t register_source);

void add_imm(uint8_t register_dest,


#endif //CPU_H
