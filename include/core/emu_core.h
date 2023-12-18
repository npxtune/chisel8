
#ifndef CHISEL8_EMU_CORE_H
#define CHISEL8_EMU_CORE_H
#include <stdint.h>
uint16_t fetch(const uint16_t *PC);
int32_t decode();
int32_t execute();
#endif //CHISEL8_EMU_CORE_H
