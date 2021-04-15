#include "pdp.h"
#include <stdio.h>
#include <stdlib.h>


extern byte mem[MEMSIZE];
extern word reg[8];


void do_halt() {
    trace("THE END!!!\n");
    exit(0);
}

void do_mov() {
    pc += 2;
    trace("mov\n");
}

void do_add() {
    trace("add\n");
}

void nothing() {
    trace("Unknown command.\n");
    exit(0);
}



Command cmd[] = {
        {0170000, 0010000, "mov",     do_mov},
        {0170000, 0060000, "add",     do_add},
        {0177777, 0000000, "halt",    do_halt},
        {0000000, 0000000, "nothing", nothing},
};

int cmd_len = sizeof(cmd) / sizeof(Command);