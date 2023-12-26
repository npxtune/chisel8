#pragma once
#include <stdint.h>
#include "raylib.h"
#include "core/emu_definition.h"
#include "core/emu_file.h"
#include "core/emu_core.h"
#include "gui/options_window.h"
void emu_stop(chip8 *system);
void check_input(chip8 *emu);
int32_t emu_main(options_config *config);