// clang-format off

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <stdio.h>

#define CYCLE_TIME_SEC  1

#define BYTE_MAX UCHAR_MAX
typedef uint8_t byte;

#define ALLOW_BREAKPOINT(a) do { \
    if (break_enabled && ((a) == break_at)) { \
            raise(SIGTRAP); \
        } \
    } while(0)

#define POP_CODE_BYTE()  g_code[i], i++

enum {
    OP_INIT,
    OP_MOV,
    OP_NOT,
    OP_AND,
    OP_OR,
    OP_OR_POS
};

byte g_data_memory[BYTE_MAX+1];

byte g_code[] = {
    OP_NOT, 0x00, 0x00,
    OP_MOV, 0x02, 0x00,
    OP_INIT, 0x01, 0xef,
    OP_INIT, 0x03, 0x01,
    OP_OR_POS, 0x01, 0x04, 0x03, 0x03
};

bool break_enabled = false;
int break_at = 0;

void dump_memory() {
    int i;
    for (i = 0; i < sizeof g_data_memory; i++) {
        printf("0x%02x ", g_data_memory[i]);
    }
    printf("\n\n");
}

void breakpoint_at(int addr) {
    break_at = addr;
    break_enabled = true;
}

int main(int argc, char const *argv[]) {
    int i = 0;
    byte op_code;
    byte dest;
    byte src1;
    byte src2;
    byte bitpos;

    /* initialize memory */
    memset(g_data_memory, 0, sizeof g_data_memory);

    // breakpoint_at(0x02);

    while (true) {
        i = 0;
        while (i < sizeof g_code) {
            op_code = POP_CODE_BYTE();
            switch (op_code) {
                case OP_INIT: {
                    dest = POP_CODE_BYTE();

                    g_data_memory[dest] = POP_CODE_BYTE();
                    ALLOW_BREAKPOINT(dest);
                }
                break;

                case OP_MOV: {
                    dest = POP_CODE_BYTE();
                    src1 = POP_CODE_BYTE();

                    g_data_memory[dest] = g_data_memory[src1];
                    ALLOW_BREAKPOINT(dest);
                }
                break;

                case OP_NOT: {
                    dest = POP_CODE_BYTE();
                    src1 = POP_CODE_BYTE();

                    g_data_memory[dest] = ~g_data_memory[src1];
                    ALLOW_BREAKPOINT(dest);
                }
                break;

                case OP_AND: {
                    dest = POP_CODE_BYTE();
                    src1 = POP_CODE_BYTE();
                    src2 = POP_CODE_BYTE();

                    g_data_memory[dest] = g_data_memory[src1] & g_data_memory[src2];
                    ALLOW_BREAKPOINT(dest);
                }
                break;

                case OP_OR: {
                    dest = POP_CODE_BYTE();
                    src1 = POP_CODE_BYTE();
                    src2 = POP_CODE_BYTE();

                    g_data_memory[dest] = g_data_memory[src1] | g_data_memory[src2];
                    ALLOW_BREAKPOINT(dest);
                }
                break;

                case OP_OR_POS: {
                    dest   = POP_CODE_BYTE();
                    bitpos = POP_CODE_BYTE();
                    src1   = POP_CODE_BYTE();
                    src2   = POP_CODE_BYTE();

                    g_data_memory[dest] |= (g_data_memory[src1] << bitpos) | (g_data_memory[src2] << bitpos);
                    ALLOW_BREAKPOINT(dest);
                }
                break;
            }


        }

        dump_memory();

        sleep(CYCLE_TIME_SEC);
    }

    return 0;
}

