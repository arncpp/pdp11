#include "pdp.h"
#include <stdio.h>
#include <stdlib.h>


extern byte mem[];
extern word reg[];
extern Command cmd[];
extern int cmd_len;

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
