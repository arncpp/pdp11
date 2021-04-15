#ifndef PDP11_PDP_H
#define PDP11_PDP_H

#define MEMSIZE (64*1024)
#define pc reg[7]

typedef unsigned char byte;        //8 bit
typedef unsigned short int word;   //16 bit
typedef word Adress;               //16 bit


void b_write(Adress adr, byte b);

byte b_read(Adress adr);

void w_write(Adress adr, word w);

word w_read(Adress adr);

void load_file(const char *filename);

void trace(char *str, ...);

void run();

typedef struct {
    word mask;
    word opcode;
    char *name;

    void (*do_func)(void);

} Command;



#endif //PDP11_PDP_H
