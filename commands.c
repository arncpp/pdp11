#include "pdp.h"
#include <stdio.h>
#include <stdlib.h>


extern byte mem[MEMSIZE];
extern word reg[8];
extern struct Argument ss, dd, nn, Rn, R, B, xx;
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
    trace("br %d", (char) xx.val * 2);
}

void do_bcc() {
    if (flag_C == 0)
        do_br();

}

void do_bcs() {
    if (flag_C == 1)
        do_br();

}

void do_beq() {
    if (flag_Z == 1)
        do_br();

}

void do_bge() {
    if ((flag_N ^ flag_V) == 0)
        do_br();

}

void do_bgt() {
    if ((flag_Z | (flag_N ^ flag_V)) == 0)
        do_br();

}

void do_bhi() {
    if ((flag_C | flag_Z) == 0)
        do_br();

}


void do_ble() {
    if ((flag_Z | (flag_N ^ flag_V)) == 1)
        do_br();

}

void do_blt() {
    if ((flag_N ^ flag_V) == 1)
        do_br();

}


void do_blos() {
    if ((flag_C | flag_Z) == 1)
        do_br();

}

void do_bmi() {
    if (flag_N == 1)
        do_br();

}

void do_bne() {
    if (flag_Z == 0)
        do_br();

}

void do_bpl() {
    if (flag_N == 0)
        do_br();

}

void do_tst() {
    if (B.val) {
        if ((dd.val >> 7) & 1) {
            flag_Z = 0;
            flag_N = 1;
        } else {
            flag_N = 0;
            if (dd.val == 0)
                flag_Z = 1;
            else
                flag_Z = 0;
        }
    } else
        set_NZ(dd.val);
    flag_C = 0;
    flag_V = 0;
}

void do_jsr(){

    word temp = dd.adr;
    w_write(sp -= 2, w_read(Rn.val));
    w_write(Rn.val, pc);
    pc = temp;

}

void do_rts() {
    pc = w_read(R.val);
    reg[R.val] = w_read(sp);
    sp += 2;

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
    if ((dd.adr == odata) && ((w_read(ostat) >> 7) & 1))
        printf("%c", ss.val);
}

void do_add() {
    int add = ss.val + dd.val;
    w_write(dd.adr, (word) add);
    set_ALL(add);
}

void do_sob() {
    if (--reg[Rn.val] != 0) {
        pc = pc - nn.val * 2;
    }
}

void do_clr() {
    w_write(dd.adr, 0);
}

void do_bic(){
    word mask = ss.val;
    word w = dd.val;
    trace("m: %06o | w: %06o | ", mask, w);
    w = ~mask & w;
    trace("res: %06o", w);
    w_write(dd.adr, w);
    set_NZ(w);
}

void do_ash(){
    word w = reg[Rn.val];
    word n = dd.val & 077;
    trace("%06o >> %o", w, n);

    if (n >> 5) { //отрицательный знак
        n = (037 + 1) - (char)(n & 037);
        trace("%d ", -n);
        for (int i =0; i < n; i++){
            flag_C = w & 1;
            word b = w >> 15;
            w = w >> 1;
            w = w | (b << 15);
        }
    }
    else { //положительный знак
        n = (char)(n & 037);
        trace("%d ", n);
        for (int i = 0; i < n; i++) {
            word b = w >> 15;
            flag_C = b & 1;
            w = w << 1;
        }
    }

    trace("res:%06o", w);
    set_NZ(w);
    w_write(Rn.val, w);
}



void nothing() {
    trace("Unknown command.\n");
    print_register();
    exit(0);
}

//1 - need DD
//2 - need SS
//3 - need SS and DD
Command cmd[] = {
        {0070000, 0010000, "mov",     do_mov,  3},
        {0170000, 0060000, "add",     do_add,  3},
        {0177777, 0000000, "halt",    do_halt, 0},
        {0177000, 0077000, "sob",     do_sob,  0},
        {0077700, 0005000, "clr",     do_clr,  1},
        {0177777, 0000250, "cln",     do_cln,  0},
        {0177777, 0000244, "clz",     do_clz,  0},
        {0177777, 0000242, "clv",     do_clv,  0},
        {0177777, 0000241, "clc",     do_clc,  0},
        {0177777, 0000257, "ccc",     do_ccc,  0},
        {0177777, 0000250, "sen",     do_sen,  0},
        {0177777, 0000244, "sez",     do_sez,  0},
        {0177777, 0000242, "sev",     do_sev,  0},
        {0177777, 0000241, "sec",     do_sec,  0},
        {0177777, 0000257, "scc",     do_scc,  0},
        {0177400, 0000400, "br",      do_br,   0},
        {0177400, 0103000, "bcc",     do_bcc,  0},
        {0177400, 0103400, "bcs",     do_bcs,  0},
        {0177400, 0001400, "beq",     do_beq,  0},
        {0177400, 0002000, "bge",     do_bge,  0},
        {0177400, 0003000, "bgt",     do_bgt,  0},
        {0177400, 0101000, "bhi",     do_bhi,  0},
        {0177400, 0003400, "ble",     do_ble,  0},
        {0177400, 0002400, "blt",     do_blt,  0},
        {0177400, 0101400, "blos",    do_blos, 0},
        {0177400, 0100400, "bmi",     do_bmi,  0},
        {0177400, 0001000, "bne",     do_bne,  0},
        {0177400, 0100000, "bpl",     do_bpl,  0},
        {0077700, 0005700, "tst",     do_tst,  1},
        {0177000, 0004000, "jsr",     do_jsr,  1},
        {0177770, 0000200, "rts",     do_rts,  0},
        {0170000, 0040000, "bic",     do_bic,  3},
        {0177000, 0072000, "ash",     do_ash,  1},
        {0000000, 0000000, "nothing", nothing, 0}
};

int cmd_len = sizeof(cmd) / sizeof(Command);

