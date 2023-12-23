
#pragma once
#include <stdio.h>
#include <stdint.h>
#include "core/emu_definition.h"
uint16_t fetch(chip8 *system);
int32_t decode_exec(uint16_t instruction, chip8 *system);