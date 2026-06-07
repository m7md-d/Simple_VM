#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "vm.h"

static int is_number(const char *s)
{
    if (!s || !*s) return 0;
    for (; *s; s++)
        if (!isdigit((unsigned char)*s)) return 0;
    return 1;
}

int main(int argc, char **argv)
{
    if (argc != 3) {
        fprintf(stderr, "usage: %s <data_file> <size | code_file>\n", argv[0]);
        return 1;
    }

    const char *data_path = argv[1];

    /**
     * second arg is a size -> carve a fresh data file, then stop.
     */
    if (is_number(argv[2])) {
        uint16_t size = (uint16_t)strtoul(argv[2], NULL, 10);
        if (creat_memory(data_path, size) != 0) {
            fprintf(stderr, "error: could not create data file '%s'\n", data_path);
            return 1;
        }
        return 0;
    }

    /**
     * second arg is a path -> it is the code file: load both spaces and run.
     */
    const char *code_path = argv[2];

    memory *mem = read_memory(data_path);
    if (!mem) {
        fprintf(stderr, "error: could not read data file '%s'\n", data_path);
        return 1;
    }

    size_t code_count = 0;
    instruction *code = read_code(code_path, &code_count);
    if (!code) {
        fprintf(stderr, "error: could not read code file '%s'\n", code_path);
        free(mem);
        return 1;
    }

    int rc = run_vm(mem, code, code_count);

    free(code);
    free(mem);
    return rc;
}
