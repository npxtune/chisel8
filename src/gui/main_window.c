
//==============================================================================
//    -  MIT License | main_window.c
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

//  RAYLIB INCLUDE
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION

#include "raygui.h"
#include "dark/style_dark.h"

#include "gui/main_window.h"

void main_window(options_config *config) {
    ui_scale scale;
    float temp;

    //  Show debug info?
    if(config->show_debug == true) {
        SetTraceLogLevel(LOG_INFO);
    } else {
        SetTraceLogLevel(LOG_ERROR);
    }

    scale.window_width = DISPLAY_WIDTH * config->display_scaling;
    scale.window_height = DISPLAY_HEIGHT * config->display_scaling;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(scale.window_width,scale.window_height,WINDOW_TITLE VERSION);

    scale.button_width = (float)(scale.window_width/4.8);
    scale.button_height = (float)((float)scale.window_height/16);
    scale.button_x = (float)((float)scale.window_width/2-(scale.window_width/9.6));

    scale.font_size = (scale.window_height/12);
    GuiLoadStyleDark();

    enum menu_state_counter {normal, options, init};
    uint32_t menu_state = normal;

    SetTargetFPS(REFRESH_RATE);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        if(IsWindowResized()) {
            config->display_scaling = GetScreenWidth() / DISPLAY_WIDTH;
            scale.window_width = DISPLAY_WIDTH * config->display_scaling;
            scale.window_height = DISPLAY_HEIGHT * config->display_scaling;
            SetWindowSize(scale.window_width, scale.window_height);

            scale.button_width = (float)(scale.window_width/4.8);
            scale.button_height = (float)((float)scale.window_height/16);
            scale.button_x = (float)((float)scale.window_width/2-(scale.window_width/9.6));
            scale.font_size = (scale.window_height/12);
        }

        if (IsFileDropped()) {  // Initialize Emulation
            EndDrawing();
            if(emu_main(config, &scale) == -2) {break;}
            menu_state = normal;
        }

        SetWindowTitle(WINDOW_TITLE VERSION);

        switch (menu_state) {
            /*-------------------------------------------------------------------------------------------------------------*/
            case (normal):
                DrawText("Chisel8 Emulator", scale.window_width/2-(scale.font_size*4), (scale.window_height/12), scale.font_size, RAYWHITE);

                if (GuiButton((Rectangle){ scale.button_x, scale.window_height/2-scale.window_height/6-10, scale.button_width, scale.button_height },
                              GuiIconText(ICON_CPU, "Load ROM") )) {
                    menu_state = init;
                }

                if (GuiButton((Rectangle){ scale.button_x, scale.window_height/2-scale.window_height/16, scale.button_width, scale.button_height },
                              GuiIconText(ICON_GEAR, "Settings"))) {
                    menu_state = options;
                }

                if (GuiButton((Rectangle){ scale.button_x, scale.window_height-(scale.window_height/6), scale.button_width, scale.button_height },
                              GuiIconText(ICON_EXIT, "Quit"))) {
                    EndDrawing();
                    CloseWindow();
                    return;
                }
                break;

            /*-------------------------------------------------------------------------------------------------------------*/

            case (options):
                // TODO: Call function from options_window source file and actually add options to choose from
                // TODO: Should allow the user to change the window scaling and other options...

                DrawText("Settings", scale.window_width/2-(scale.font_size*2), (scale.window_height/12), scale.font_size, RAYWHITE);

                if (config->volume != 0) {
                    if (GuiButton((Rectangle){ scale.button_x, scale.window_height-(scale.window_height/2), scale.button_width, scale.button_height },
                                  GuiIconText(ICON_AUDIO, "Mute Audio"))) {
                        temp = config->volume;
                        config->volume = 0.0f;
                    }
                } else {
                    if (GuiButton((Rectangle){ scale.button_x, scale.window_height-(scale.window_height/2), scale.button_width, scale.button_height },
                                  GuiIconText(ICON_AUDIO, "Unmute Audio"))) {
                        config->volume = temp;
                    }
                }

                if (GuiButton((Rectangle){ scale.button_x, scale.window_height-(scale.window_height/6), scale.button_width, scale.button_height },
                              GuiIconText(ICON_REREDO_FILL, "Return"))) {
                    menu_state = normal;
                    break;
                }
                break;

            /*-------------------------------------------------------------------------------------------------------------*/

            case (init):
                DrawRectangle(0, 0, scale.window_width, scale.window_height, Fade(RAYWHITE, 0.2f));
                DrawText("Please drag a ROM file into the window", scale.window_width/2-(scale.font_size*10)+(scale.font_size/2),
                         (scale.window_height/2)-scale.font_size*1.5, scale.font_size, RAYWHITE);

                if (GuiButton((Rectangle){ scale.button_x, scale.window_height-(scale.window_height/6), scale.button_width, scale.button_height },
                              GuiIconText(ICON_REREDO_FILL, "Return"))) {
                    menu_state = normal;
                    break;
                }
                if (IsFileDropped()) {  // Initialize Emulation
                    EndDrawing();
                    if(emu_main(config, &scale) == -2) {break;}
                    menu_state = normal;
                    break;
                }
                break;

            /*-------------------------------------------------------------------------------------------------------------*/
        }
        EndDrawing();
    }
    CloseWindow();
}