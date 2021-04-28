#include "pdp.h"
#include <stdio.h>
#include <stdlib.h>


extern byte mem[MEMSIZE];
extern word reg[8];
extern struct Argument ss, dd, nn, Rn, B;


void do_halt() {
    trace("THE END!!!\n");
    print_register();
    exit(0);
}

void do_mov() {
    if (B.val)
        b_write(dd.adr, (byte) ss.val);
    else
        w_write(dd.adr, ss.val);
    trace("mov\n");
}

void do_add() {
    word add = ss.val + dd.val;
    w_write(dd.adr, add);
    trace("add\n");
}

void do_sob() {
    if (--reg[Rn.val] != 0) {
        pc = pc - nn.val * 2;
    }
    trace("sob\n");
}

void do_clr() {
    w_write(dd.adr, 0);
    trace("clr\n");
}

void nothing() {
    trace("Unknown command.\n");
    print_register();
    exit(0);
}

// params: 0BXRNSD
Command cmd[] = {
        {0070000, 0010000, "mov",     do_mov,  0100011},
        {0170000, 0060000, "add",     do_add,  0000011},
        {0177777, 0000000, "halt",    do_halt, 0000000},
        {0177000, 0077000, "sob",     do_sob,  0001100},
        {0077700, 0005000, "clr",     do_clr,  0100001},
        {0000000, 0000000, "nothing", nothing, 0000000}
};

int cmd_len = sizeof(cmd) / sizeof(Command);