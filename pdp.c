#include <stdio.h>
#include "pdp.h"
#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>


byte mem[MEMSIZE];
word reg[REGSIZE];

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
    for (int i = 01000;;) {
        word r = w_read(i);
        printf("%06o: %06o\n", i, r);
        if (r == 0)
            break;
        i += 2;

    }

}


int main(int argc, char *argv[]) {
    load_file(argv[1]);
    print_mem();
    run();
    print_register();
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
    if (adr < 8) {
        reg[adr] = b;
        if (b >> 7)
            reg[adr] = 0xFF00 + b;
        else
            reg[adr] = b;
    } else {
        mem[adr] = b;
        if (b >> 7)
            mem[adr + 1] = 0xFF;
        else
            mem[adr + 1] = 0;

    }

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

void trace(char *format, ...) {
    va_list v_list;
            va_start(v_list, format);
    vprintf(format, v_list);
            va_end(v_list);

}

void print_register() {
    for (int i = 0; i < REGSIZE; i++) {
        printf("R[%d]: %06o\n", i, reg[i]);

    }

}
