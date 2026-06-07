#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vm.h"
#include "execute.h"

int creat_memory(const char *path, uint16_t data_size)
{
    if (!data_size) return -1;

    FILE *f = fopen(path, "wb");
    if (!f) return -1;

    memory mem;

    mem.meta.magic_n = VM_MAGIC;
    mem.meta.version = 1;
    mem.meta.data_size = data_size;


    if (fwrite(&mem.meta, sizeof(mem.meta), 1, f) != 1) {
        fclose(f);
        return -1;
    }

    uint8_t *zero = calloc(data_size, 1);
    if (!zero) {
        fclose(f);
        return -1;
    }
    if (fwrite(zero, 1, data_size, f) != data_size) {
        free(zero);
        fclose(f);
        return -1;
    }

    free(zero);
    fclose(f);
    return 0;
}

memory *read_memory(const char *path)
{
    FILE *f = fopen(path, "rb");
    if (!f) return NULL;

    memory tmp;
    if (fread(&tmp.meta, sizeof(tmp.meta), 1, f) != 1) {
        fclose(f);
        return NULL;
    }

    if (tmp.meta.magic_n != VM_MAGIC) {
        fclose(f);
        return NULL;
    }

    memory *mem = malloc(sizeof(memory) + tmp.meta.data_size);
    if (!mem) {
        fclose(f);
        return NULL;
    }
    mem->meta = tmp.meta;

    if (fread(mem->value, 1, mem->meta.data_size, f) != mem->meta.data_size) {
        free(mem);
        fclose(f);
        return NULL;
    }

    fclose(f);
    return (mem);
}

instruction *read_code(const char *path, size_t *code_count)
{
    FILE *f = fopen(path, "rb");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    *code_count = file_size / sizeof(instruction);

    instruction *code_array = calloc(file_size, 1);
    if (!code_array) {
        fclose(f);
        return NULL;
    }

    fread(code_array, sizeof(instruction), *code_count, f);

    fclose(f);
    return code_array;
}

int run_vm(memory *mem, instruction *code, size_t code_count)
{
    vm vm1 = {0};
    vm1.mem = mem;

    while (vm1.ip < code_count) {
        if (fetch(&vm1, code) != 0)     break;
        if (decode(vm1.ir) != 0)        break;
        if (execute(&vm1) != 0)         break;
        if (writeback(&vm1) != 0)       break;
    }

    return (0);
}

int fetch(vm* vir, instruction *code)
{
    memcpy(&vir->ir, &code[vir->ip], sizeof(vir->ir));
    vir->ip++;
    return (0);
}

int decode(instruction ir)
{
    if (ir.opcode >= OPCODE_COUNT) return (-1);
    return (0);
}

int writeback(vm *v)
{
    /* only stores reach the data space; everything else committed in execute. */
    if (v->ir.opcode != OPCODE_SW) return (0);

    /* isolation: a store must explicitly flag the data space. */
    if (!v->ir.is_data || !v->mem) return (-1);

    uint16_t *addr_r = get_register(v, v->ir.r0);
    uint16_t *val_r  = get_register(v, v->ir.r1);
    if (!addr_r || !val_r) return (-1);

    uint16_t addr = *addr_r;
    if ((size_t)addr + 1 >= v->mem->meta.data_size) return (-1);

    v->mem->value[addr]     = (uint8_t)(*val_r & 0xFF);
    v->mem->value[addr + 1] = (uint8_t)(*val_r >> 8);
    return (0);
}
