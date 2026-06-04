#include <stdio.h>
#include <stdlib.h>
#include "vm.h"



int main(int argc, char **argv)
{
    FILE *file = fopen(argv[1], "wrb");
    run_vm(file);
    return (0);
}
