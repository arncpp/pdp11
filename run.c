#include "pdp.h"
#include <stdio.h>
#include <stdlib.h>


extern byte mem[];
extern word reg[];
extern Command cmd[];
extern int cmd_len;

struct Argument ss, dd, nn, Rn, B, xx, R;
char flag_Z, flag_N, flag_C, flag_V;

struct Argument mode(word opcode) {
    struct Argument result;
    int reg_number = opcode & 7;
    char mode_number = (opcode >> 3) & 7;
    int x;
    switch (mode_number) {
        case 0:                     //Rn
            result.adr = reg_number;
            result.val = reg[result.adr];
            trace("mode 0, result adr: %o, result val: %o\n", result.adr, result.val);
            break;
        case 1:                     //(Rn)
            result.adr = reg[reg_number];
            result.val = w_read(result.adr);
            trace("mode 1, result adr: %o, result val: %o \n", result.adr, result.val);
            break;
        case 2:                     //(Rn)+
            result.adr = reg[reg_number];
            result.val = w_read(result.adr);
            if (B.val && reg_number != 7)
                reg[reg_number] += 1;
            else
                reg[reg_number] += 2;
            trace("mode 2, result adr: %o, result val: %o \n", result.adr, result.val);
            break;
        case 3:                    //@(Rn)+
            result.adr = reg[reg_number];
            result.adr = w_read(result.adr);
            result.val = w_read(result.adr);
            reg[reg_number] += 2;
            trace("mode 3, result adr: %o, result val: %o \n", result.adr, result.val);
            break;
        case 4:
            reg[reg_number] -= 2;
            result.adr = reg[reg_number];
            result.val = w_read(result.adr);
            trace("mode 4, result adr: %o, result val: %o \n", result.adr, result.val);
            break;
        case 5:
            reg[reg_number] -= 2;
            result.adr = reg[reg_number];
            result.adr = w_read(result.adr);
            result.val = w_read(result.adr);
            trace("mode 5, result adr: %o, result val: %o \n", result.adr, result.val);
            break;
        case 6:
            x = w_read(pc);
            pc += 2;
            result.adr = x + reg[reg_number];
            result.val = w_read(result.adr);
            trace("mode 6, result adr: %o, result val: %o \n", result.adr, result.val);
            break;
        case 7:
            x = w_read(pc);
            pc += 2;
            result.adr = x + reg[reg_number];
            result.adr = w_read(result.adr);
            result.val = w_read(result.adr);
            trace("mode 7, result adr: %o, result val: %o \n", result.adr, result.val);
            break;
    }
    return result;
}

int get_ss(Command comm) {
    return (comm.params & 2) == 2;
}

int get_dd(Command comm) {
    return (comm.params & 1) == 1;
}

void get_nn(word w) {
    nn.val = w & 077;
}

void get_Rn(word w) {
    Rn.val = (w >> 6) & 7;
}

void get_B(word w) {
    B.val = (w >> 15) & 1;
}

void get_xx(word w) {
    xx.val = (w & 0xFF);
}

void get_R(word w) {
    R.val = w & 7;
}

void run() {
    pc = 01000;
    w_write(ostat, 0x80);
    while (pc != 0) {
        word w = w_read(pc);
        trace("%06o %06o: ", pc, w);
        pc += 2;
        for (int i = 0; i < cmd_len; i++) {
            if ((w & cmd[i].mask) == cmd[i].opcode) {
                get_B(w);
                trace(cmd[i].name);
                trace(" ");
                if (get_ss(cmd[i])) {
                    ss = mode(w >> 6);
                }
                if (get_dd(cmd[i])) {
                    dd = mode(w);
                }
                //print_register();
                get_nn(w);
                get_Rn(w);
                get_R(w);
                get_xx(w);

                cmd[i].do_func();
                printf("\n");
                break;
            }
        }
    }
}
