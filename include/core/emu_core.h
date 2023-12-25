
#pragma once
#include <stdio.h>
#include <stdint.h>
#include "core/emu_definition.h"
#include "gui/options_window.h"
int32_t fetch(chip8 *system);
int32_t decode_exec(chip8 *emu, options_config *config);