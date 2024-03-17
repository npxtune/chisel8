
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

    if (config->show_debug == true) {    //  Show debug info?
        SetTraceLogLevel(LOG_INFO);
    } else {
        SetTraceLogLevel(LOG_WARNING);
    }

    ui_scale scale;
    scale.window_width = DISPLAY_WIDTH * config->display_scaling;
    scale.window_height = DISPLAY_HEIGHT * config->display_scaling;
    scale.emu_width = DISPLAY_WIDTH * config->display_scaling;
    scale.emu_height = DISPLAY_HEIGHT * config->display_scaling;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(scale.window_width, scale.window_height, WINDOW_TITLE VERSION);

    scale.button_width = (float) (scale.window_width / 4.8);
    scale.button_height = (float) ((float) GetScreenHeight() / 16);
    scale.button_x = (float) ((float) scale.window_width / 2 - (scale.window_width / 9.6));

    scale.font_size = (GetScreenHeight() / (int32_t) config->display_scaling);
    GuiLoadStyleDark();
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, 1023);   //  Set window box colour, no idea what the value is, but it works

    bool is_options = false;

    SetTargetFPS(REFRESH_RATE);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        //----------------------------------------------------------------------------------
        //  WINDOW EVENTS
        //----------------------------------------------------------------------------------

        if (IsKeyPressed(KEY_F11)) {
            if (!IsWindowFullscreen()) {
                SetWindowSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
                ToggleFullscreen();
            } else {
                ToggleFullscreen();
                SetWindowSize(scale.window_width, scale.window_height);
            }
        }

        if (IsWindowResized()) {    //  Magic to enable dynamic scaling
            config->display_scaling = (uint32_t) fminf(GetScreenWidth() / (float) DISPLAY_WIDTH,
                                                       GetScreenHeight() / (float) DISPLAY_HEIGHT);

            if (!IsWindowFullscreen()) {
                scale.window_width = GetScreenWidth();
                scale.window_height = GetScreenHeight();
            }

            scale.emu_width = DISPLAY_WIDTH * config->display_scaling;
            scale.emu_height = DISPLAY_HEIGHT * config->display_scaling;

            scale.button_width = (float) (GetScreenWidth() / 4.8);
            scale.button_height = (float) ((float) GetScreenHeight() / 16);
            scale.button_x = (float) ((float) GetScreenWidth() / 2 - (GetScreenWidth() / 9.6));
            scale.font_size = (scale.emu_width / scale.emu_height) * (int32_t) config->display_scaling;

            GuiSetStyle(DEFAULT, TEXT_SIZE, (int32_t) (scale.font_size / 1.8));
            GuiSetIconScale((int32_t) (scale.font_size / 1.8) / 16);
        }

        if (IsFileDropped()) {  // Initialize Emulation
            EndDrawing();
            if (emu_main(config, &scale) == -2) { break; }
        }
        SetWindowTitle(WINDOW_TITLE VERSION);

        //----------------------------------------------------------------------------------
        //  RENDER LOGIC
        //----------------------------------------------------------------------------------

        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(RAYWHITE, 0.2f));
        DrawText("Please drag a ROM file into the window",
                 GetScreenWidth() / 2 - (scale.font_size * 10) + (scale.font_size / 2),
                 (GetScreenHeight() / 2) - scale.font_size, scale.font_size, RAYWHITE);

        if (GuiButton((Rectangle) {GetScreenWidth() - scale.button_height * 4.5,
                                   GetScreenHeight() - scale.button_height * 1.5,
                                   scale.button_height, scale.button_height},
                      GuiIconText(ICON_INFO, ""))) {
            OpenURL("https://github.com/npxtune/chisel8");
        }

        if (GuiButton((Rectangle) {GetScreenWidth() - scale.button_height * 3,
                                   GetScreenHeight() - scale.button_height * 1.5,
                                   scale.button_height, scale.button_height},
                      GuiIconText(ICON_GEAR, "")) || IsKeyPressed(KEY_S)) {
            if (is_options) {
                is_options = false;
            } else {
                is_options = true;
            }
        }

        if (GuiButton((Rectangle) {GetScreenWidth() - scale.button_height * 1.5,
                                   GetScreenHeight() - scale.button_height * 1.5,
                                   scale.button_height, scale.button_height},
                      GuiIconText(ICON_EXIT, ""))) {
            EndDrawing();
            CloseWindow();
            return;
        }

        if (is_options) {
            SetExitKey(KEY_NULL);
            if (GuiWindowBox((Rectangle) {(GetScreenWidth() / 5) / 2, (GetScreenHeight() / 5) / 2,
                                          GetScreenWidth() - GetScreenWidth() / 5,
                                          GetScreenHeight() - GetScreenHeight() / 4.5}, "")) {
                is_options = false;
            }
            options_window(config, &scale) != 0 ? is_options = false : 0;
        } else {
            SetExitKey(KEY_ESCAPE);
        }
        EndDrawing();
    }
    CloseWindow();
}