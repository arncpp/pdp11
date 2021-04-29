#include "pdp.h"
#include <stdio.h>
#include <stdlib.h>


extern byte mem[MEMSIZE];
extern word reg[8];
extern struct Argument ss, dd, nn, Rn, B, xx;
extern char flag_Z, flag_N, flag_C, flag_V;

void set_NZ(word w) {
    if (w >> 15) {
        flag_Z = 0;
        flag_N = 1; // положительное
    } else {
        flag_N = 0;
        if (w == 0)
            flag_Z = 1; //если ноль
        else
            flag_Z = 0;
    }

}

void set_ALL(int res) {
    set_NZ((word) res);
    flag_C = (res >> 16) & 1;

}

void do_cln() {
    flag_N = 0;
}

void do_clz() {
    flag_Z = 0;
}

void do_clv() {
    flag_V = 0;
}

void do_clc() {
    flag_C = 0;
}

void do_ccc() {
    flag_N = 0;
    flag_Z = 0;
    flag_V = 0;
    flag_C = 0;
}

void do_sen() {
    flag_N = 1;
}

void do_sez() {
    flag_Z = 1;
}

void do_sev() {
    flag_V = 1;
}

void do_sec() {
    flag_C = 1;
}

void do_scc() {
    flag_N = 1;
    flag_Z = 1;
    flag_V = 1;
    flag_C = 1;
}

void do_br() {
    pc += (char) xx.val * 2;
    trace("br %d\n", (char) xx.val * 2);
}

void do_bcc() {
    if (flag_C == 0)
        do_br();
    trace("do_bcc\n");
}

void do_bcs() {
    if (flag_C == 1)
        do_br();
    trace("do_bcs\n");
}

void do_beq() {
    if (flag_Z == 1)
        do_br();
    trace("do_beq\n");
}

void do_bge() {
    if ((flag_N ^ flag_V) == 0)
        do_br();
    trace("do_bge\n");
}

void do_bgt() {
    if ((flag_Z | (flag_N ^ flag_V)) == 0)
        do_br();
    trace("do_bgt\n");
}

void do_bhi() {
    if ((flag_C | flag_Z) == 0)
        do_br();
    trace("do_bhi\n");
}


void do_ble() {
    if ((flag_Z | (flag_N ^ flag_V)) == 1)
        do_br();
    trace("do_ble\n");
}

void do_blt() {
    if ((flag_N ^ flag_V) == 1)
        do_br();
    trace("do_blt\n");
}


void do_blos() {
    if ((flag_C | flag_Z) == 1)
        do_br();
    trace("do_blos\n");
}

void do_bmi() {
    if (flag_N == 1)
        do_br();
    trace("do_bmi\n");
}

void do_bne() {
    if (flag_Z == 0)
        do_br();
    trace("do_bne\n");
}

void do_bpl() {
    if (flag_N == 0)
        do_br();
    trace("do_bpl\n");
}

void do_tst() {
    if (B.val)
        set_NZ(b_read(dd.adr));
    else
        set_NZ(w_read(dd.adr));
    flag_C = 0;
    flag_V = 0;
}


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
    set_NZ(w_read(dd.adr));
    trace("mov\n");
}

void do_add() {
    int add = ss.val + dd.val;
    w_write(dd.adr, (word) add);
    set_ALL(add);
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
        {0177777, 0000250, "cln",     do_cln,  0000000},
        {0177777, 0000244, "clz",     do_clz,  0000000},
        {0177777, 0000242, "clv",     do_clv,  0000000},
        {0177777, 0000241, "clc",     do_clc,  0000000},
        {0177777, 0000257, "ccc",     do_ccc,  0000000},
        {0177777, 0000250, "sen",     do_sen,  0000000},
        {0177777, 0000244, "sez",     do_sez,  0000000},
        {0177777, 0000242, "sev",     do_sev,  0000000},
        {0177777, 0000241, "sec",     do_sec,  0000000},
        {0177777, 0000257, "scc",     do_scc,  0000000},
        {0177400, 0000400, "br",      do_br,   0000000},
        {0177400, 0103000, "bcc",     do_bcc,  0010000},
        {0177400, 0103400, "bcs",     do_bcs,  0010000},
        {0177400, 0001400, "beq",     do_beq,  0010000},
        {0177400, 0002000, "bge",     do_bge,  0010000},
        {0177400, 0003000, "bgt",     do_bgt,  0010000},
        {0177400, 0101000, "bhi",     do_bhi,  0010000},
        {0177400, 0003400, "ble",     do_ble,  0010000},
        {0177400, 0002400, "blt",     do_blt,  0010000},
        {0177400, 0101400, "blos",    do_blos, 0010000},
        {0177400, 0100400, "bmi",     do_bmi,  0010000},
        {0177400, 0001000, "bne",     do_bne,  0010000},
        {0177400, 0100000, "bpl",     do_bpl,  0010000},
        {0077700, 0005700, "tst",     do_tst,  0100001},
        {0000000, 0000000, "nothing", nothing, 0000000}
};

int cmd_len = sizeof(cmd) / sizeof(Command);

