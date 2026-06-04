#ifndef VM_H
#define VM_H

typedef unsigned char unit8;
typedef unsigned short unit16;

typedef struct memory
{
    unit8 *value;

    unit16 data_base;
    unit16 data_size;

    unit16 code_base;
    unit16 code_size;

    unit8 ofset;
}memory;

typedef struct vm
{
    unit16 ip;
    unit16 ir;
    unit16 r0;
    unit16 r1;
    unit16 r2;
    unit16 r3;
}vm;

#endif
