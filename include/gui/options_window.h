#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "raylib.h"

typedef struct Options_config{
    Color background_color;
    Color pixel_color;
    uint32_t display_scaling;
    bool show_debug;
    bool show_fps;
    uint16_t main_volume;
} options_config;

void create_config(options_config *config);
void load_settings(options_config *config);
