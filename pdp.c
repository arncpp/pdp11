#include <stdio.h>
#include "pdp.h"
#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>


byte mem[MEMSIZE];
word reg[8];

void test_mem() {
    //пишем байт, читаем байт
    byte b0 = 0x0a;
    b_write(2, b0);
    byte bres = b_read(2);
    printf("%02hhx == %02hhx\n", b0, bres);
    assert(b0 == bres);
    //пишем 2 байта читаем слово
    Adress a = 4;
    byte b1 = 0xcb;
    word w = 0xcb0a;
    b_write(a, b0);
    word b0res = b_read(a);
    word b1res = b_read(a + 1);
    b_write(a + 1, b1);
    word wres = w_read(a);
    printf("ww/br \t %04hx=%02hhx%02hhx\n", wres, b1, b0);
    assert(wres == w);
}

void print_mem() {
    for (int i = 0x0200;; i++) {
        byte r = b_read(i);
        printf("%02hhx\n", r);
        if (r == 0)
            break;

    }

}


int main() {
    load_file("tests/test_3_sum/sum.o");
    run();
    // test_mem();
    return 0;
}


void load_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("File is empty");
        //exit(errno);
    }

    Adress adr;
    word N;
    byte b;
    while (!feof(file)) {
        fscanf(file, "%hx", &adr);
        fscanf(file, "%hx", &N);
        for (int i = 0; i < N; i++) {
            fscanf(file, "%hhx", &b);
            b_write(adr, b);
            adr++;
        }
    }

    if (fclose(file) == EOF)
        printf("error\n");
    else printf("done\n");
}


void b_write(Adress adr, byte b) {
    mem[adr] = b;
}


byte b_read(Adress adr) {
    return mem[adr];
}

void w_write(Adress adr, word w) {
    if (adr < 8) {
        reg[adr] = w;
    } else {
        mem[adr] = w & 0xFF;
        mem[adr + 1] = w >> 8;
    }
}

word w_read(Adress adr) {
    if (adr < 8) {
        return reg[adr];
    } else {
        word w = mem[adr + 1] << 8;
        w = w | mem[adr];
        return w;
    }
}

void trace(char *str, ...) {
    va_list str_l;
            va_start(str_l, str);
    vprintf(str, str_l);
            va_end(str_l);

}