
//==============================================================================
//    -  MIT License | options_window.c
//
//      Copyright (c) 2023-2024, Tim <npxtune@scanf.dev>
//      All rights served.
//
//     Permission is hereby granted, free of charge, to any person obtaining a copy
//     of this software and associated documentation files (the "Software"), to deal
//     in the Software without restriction, including without limitation the rights
//     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//     copies of the Software, and to permit persons to whom the Software is
//     furnished to do so, subject to the following conditions:
//
//     The above copyright notice and this permission notice shall be included in all
//     copies or substantial portions of the Software.
//
//     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//     SOFTWARE.
//==============================================================================

#include "gui/options_window.h"
#include "raygui.h"

//  DEFAULT SETTINGS VALUES

#define color_0     "  0,  0,  0, 1;    # Background color\n"       // Default color is BLACK
#define color_1     "255,255,255, 1;    # Pixel color\n"            // Default color is WHITE
#define scaling       "15;                # Display scaling, Chip8 Display -> 32x64 Pixels\n"
#define debug       "false;             # Show debugging info in terminal\n"
#define fps         "false;             # Show Frames per Second in the top left\n"
#define audio       "0.50;               # Set the volume of the Chip-8's beep\n"

#define items 6     // How many options are configurable?

void create_config(options_config *config) {
    FILE *file = fopen("./chisel8-settings.txt", "w");

    fwrite(color_0, strlen(color_0), 1, file);
    fwrite(color_1, strlen(color_1), 1, file);
    fwrite(scaling, strlen(scaling), 1, file);
    fwrite(debug, strlen(debug), 1, file);
    fwrite(fps, strlen(fps), 1, file);
    fwrite(audio, strlen(audio), 1, file);

    config->background_color = BLACK;
    config->pixel_color = WHITE;
    config->display_scaling = 15;
    config->show_debug = false;
    config->show_fps = false;
    config->volume = 0.50f;

    fclose(file);
}

void load_settings(options_config *config) {
    FILE *file = fopen("./chisel8-settings.txt", "r");
    if (file != NULL) {
        // READ OPTIONS
        char line[100];
        int32_t temp;
        for (int32_t i = 0; i < items; ++i) {
            fgets(line, 100, file);
            uint16_t color[4] = {0, 0, 0, 0};
            uint32_t counter = 0;

            // Skip line if it is not a value
            if (line[0] == '#' || line[0] == '\n' || line[0] == EOF) {
                i -= 1;
                continue;
            }

            switch (i) {
                case 0:
                    for (int32_t j = 0; line[j] != ';'; ++j) {
                        if (isdigit(line[j]) && color[counter] > 0) {
                            color[counter] = (color[counter] * 10) + line[j] - '0';
                        } else if (isdigit(line[j])) {
                            color[counter] = line[j] - '0';
                        }
                        if (line[j] == ',') {
                            counter++;
                        }
                    }
                    config->background_color = (Color) {color[0], color[1], color[2], color[3] * 255};
                    break;

                case 1:
                    for (int32_t j = 0; line[j] != ';'; ++j) {
                        if (isdigit(line[j]) && color[counter] > 0) {
                            color[counter] = (color[counter] * 10) + line[j] - '0';
                        } else if (isdigit(line[j])) {
                            color[counter] = line[j] - '0';
                        }
                        if (line[j] == ',') {
                            counter++;
                        }
                    }
                    config->pixel_color = (Color) {color[0], color[1], color[2], color[3] * 255};
                    break;

                case 2:
                    for (int32_t j = 0; line[j] != ';'; ++j) {
                        if (isdigit(line[j]) && color[0] > 0) {
                            color[0] = (color[0] * 10) + line[j] - '0';
                        } else if (isdigit(line[j])) {
                            color[0] = line[j] - '0';
                        }
                    }
                    config->display_scaling = color[0];
                    break;

                case 3:
                    for (temp = 0; line[temp] != ';'; ++temp) {}
                    if (strncmp(&line[0], "true", sizeof(line[temp])) == 0) {
                        config->show_debug = true;
                    } else {
                        config->show_debug = false;
                    }
                    break;

                case 4:
                    for (temp = 0; line[temp] != ';'; ++temp) {}
                    if (strncmp(&line[0], "true", sizeof(line[temp])) == 0) {
                        config->show_fps = true;
                    } else {
                        config->show_fps = false;
                    }
                    break;

                case 5:
                    for (temp = 0; line[temp] != ';'; ++temp) {}
                    if (strncmp(&line[0], "0.", sizeof(line[temp])) == 0) {
                        config->volume = (float) (line[2] - '0') / 10 + (float) (line[3] - '0') / 100;
                    } else {
                        config->volume = 1.0f;
                    }
                    break;

                default:
                    fclose(file);
                    return;
            }
        }
    } else {
        // CREATE OPTIONS
        create_config(config);
    }
    fclose(file);
}

int32_t write_settings(options_config *config) {
    //  TODO
    return 0;
}

float temp;

int32_t options_window(options_config *config, ui_scale *scale) {

    SetExitKey(KEY_NULL);

    DrawText("Settings", GetScreenWidth() / 2 - (scale->font_size * 2), (GetScreenHeight() / 12),
             scale->font_size, RAYWHITE);

    //      AUDIO VOLUME SLIDER
    GuiSliderBar((Rectangle) {(scale->window_width / 2) - 250, scale->window_height / 4, 500, 25},
                 GuiIconText(ICON_AUDIO, "Volume "),
                 TextFormat("%d %%", (int32_t) (config->volume * 100)), &config->volume, 0, 1);

    //  TODO: THIS IS VERY BROKEN!!!        HIDE COLOR PICKER BEHIND BUTTON?

    GuiColorPicker((Rectangle) {100, 175, 200, 200}, "TEXT", &config->background_color);

    GuiColorPicker((Rectangle) {600, 175, 200, 200}, "TEXT", &config->pixel_color);

    GuiCheckBox((Rectangle) {scale->button_x, GetScreenHeight() - (GetScreenHeight() / 3),
                                 scale->button_width/8, scale->button_width/8}, "Show Debug info", &config->show_debug);

    GuiCheckBox((Rectangle) {scale->button_x, GetScreenHeight() - (GetScreenHeight() / 2),
                             scale->button_width/8, scale->button_width/8}, "Show FPS", &config->show_fps);


    if (GuiButton((Rectangle) {scale->button_x, GetScreenHeight() - (GetScreenHeight() / 6),
                               scale->button_width, scale->button_height},
                  GuiIconText(ICON_REREDO_FILL, "Return")) || IsKeyPressed(KEY_ESCAPE)) {
        SetExitKey(KEY_ESCAPE);
        return 0;
    }

    if (config->show_debug == true) {
        SetTraceLogLevel(LOG_INFO);
    } else {
        SetTraceLogLevel(LOG_ERROR);
    }

    return 1;
}