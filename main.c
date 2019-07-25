// clang-format off

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>

#include <stdio.h>

#define CYCLE_TIME_SEC  1

#define BYTE_MAX UCHAR_MAX
typedef uint8_t byte;

enum {
    OP_MOV,
    OP_NOT,
    OP_AND,
    OP_OR
};

byte g_data_memory[BYTE_MAX+1];

byte g_code[] = {
    OP_NOT, 0x00, 0x00,
    OP_MOV, 0x02, 0x00
};

void dump_memory() {
    int i;
    for (i = 0; i < sizeof g_data_memory; i++) {
        printf("0x%02x ", g_data_memory[i]);
    }
    printf("\n\n");
}

int main(int argc, char const *argv[]) {
    int i = 0;
    byte op_code;
    byte dest;
    byte src1;
    byte src2;

    /* initialize memory */
    memset(g_data_memory, 0, sizeof g_data_memory);

    while (true) {

        for (i = 0; i < sizeof g_code; i++) {
            op_code = g_code[i], i++;
            switch (op_code) {
                case OP_MOV: {
                    dest = g_code[i]; i++;
                    src1 = g_code[i];
                    g_data_memory[dest] = g_data_memory[src1];
                }
                break;

                case OP_NOT: {
                    dest = g_code[i]; i++;
                    src1 = g_code[i];
                    g_data_memory[dest] = ~g_data_memory[src1];
                }
                break;

                case OP_AND: {
                    dest = g_code[i]; i++;
                    src1 = g_code[i]; i++;
                    src2 = g_code[i]; i++;
                    g_data_memory[dest] = g_data_memory[src1] & g_data_memory[src2];
                }
                break;

                case OP_OR: {
                    dest = g_code[i]; i++;
                    src1 = g_code[i]; i++;
                    src2 = g_code[i]; i++;
                    g_data_memory[dest] = g_data_memory[src1] | g_data_memory[src2];
                }
                break;
            }
        }

        dump_memory();

        sleep(CYCLE_TIME_SEC);
    }

    return 0;
}
