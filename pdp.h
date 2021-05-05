#ifndef PDP11_PDP_H
#define PDP11_PDP_H

#define MEMSIZE (64*1024)
#define REGSIZE 8
#define pc reg[7]
#define odata 0177566
#define ostat 0177564
#define sp reg[6]

typedef unsigned char byte;        //8 bit
typedef unsigned short int word;   //16 bit
typedef word Adress;               //16 bit


void b_write(Adress adr, byte b);

byte b_read(Adress adr);

void w_write(Adress adr, word w);

word w_read(Adress adr);

void load_file(const char *filename);

void trace(char *st, ...);

void print_register();

void run();

typedef struct {
    word mask;
    word opcode;
    char *name;

    void (*do_func)(void);

    word params;

} Command;

struct Argument {
    word val;
    word adr;
};


#endif //PDP11_PDP_H
