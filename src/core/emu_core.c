#include "core/emu_core.h"

uint16_t fetch(const uint16_t *PC) {
    uint16_t temp = 0;
    temp = (PC[0] * 10) + PC[1];
    PC += 2;
    return temp;
}

int32_t decode() {

}

int32_t execute() {

}