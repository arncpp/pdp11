#include "pdp.h"
#include <stdio.h>
#include <stdlib.h>


extern byte mem[];
extern word reg[];
extern Command cmd[];
extern int cmd_len;

struct Argument {
    word val;
    word adr;
} ss, dd;

struct Argument mode(word opcode) {
    struct Argument result;
    int reg_number = opcode & 7;
    char mode_number = (opcode >> 3) & 7;
    switch (mode_number) {
        case 0:                     //Rn
            result.adr = reg_number;
            result.val = reg[result.adr];
            trace("result adr: %d, result val: %d \n", result.adr, result.val);
            break;
        case 1:                     //(Rn)
            result.adr = reg[reg_number];
            result.val = w_read(result.adr);
            trace("result adr: %d, result val: %d \n", result.adr, result.val);
            break;
        case 2:                     //(Rn)+
            result.adr = reg[reg_number];
            result.val = w_read(result.adr);
            reg[reg_number] += 2;
            trace("result adr: %d, result val: %d \n", result.adr, result.val);
            break;
        case 3:                    //@(Rn)+
            result.adr = reg[reg_number];
            result.adr = w_read(result.adr);
            result.val = w_read(result.adr);
            reg[reg_number] += 2;
            trace("result adr: %d, result val: %d \n", result.adr, result.val);
            break;
        case 4:
            reg[reg_number] -= 2;
            result.adr = reg[reg_number];
            result.val = w_read(result.adr);
            break;
        case 5:
            reg[reg_number] -= 2;
            result.adr = reg[reg_number];
            result.adr = w_read(result.adr);
            result.val = w_read(result.adr);
            break;
        case 6:
            int X = w_read(pc);
            pc += 2;
            result.adr = X + reg[reg_number];
            result.val = w_read(result.adr);
            break;
        case 7:
            int x = w_read(pc);
            pc += 2;
            result.adr = x + reg[reg_number];
            result.adr = w_read(result.adr);
            result.val = w_read(result.adr);
            break;
    }
    return result;
}

void run() {
    pc = 01000;
    while (1) {
        word w = w_read(pc);
        trace("%06o %06o: ", pc, w);
        pc += 2;
        for (int i = 0; i < cmd_len; i++) {
            if ((w & cmd[i].mask) == cmd[i].opcode) {
                cmd[i].do_func();
                break;
            }
        }
    }
}
