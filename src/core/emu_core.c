#include "core/emu_core.h"

void fetch(uint8_t **PC) {
    printf("EMU_CORE: Fetched: %02x", (int) **PC);
    *PC+=1;
    printf("%02x\n", (int) **PC);
    *PC+=1;
}

int32_t decode() {

}

int32_t execute() {

}