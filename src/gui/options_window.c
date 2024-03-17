
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

bool is_theme = true, is_misc = false;

void load_settings(options_config *config) {
    config->background_color = BLACK;
    config->pixel_color = WHITE;
    config->display_scaling = 15;
    config->show_debug = false;
    config->show_fps = false;
    config->volume = 0.25f;
}

int32_t options_window(options_config *config, ui_scale *scale) {
    SetExitKey(KEY_NULL);

//    if (fileDialogState.windowActive) GuiLock();

    //      AUDIO VOLUME SLIDER
    GuiSliderBar((Rectangle) {scale->button_x - scale->button_width / 1.45,
                              GetScreenHeight() - (GetScreenHeight() / 1.3), scale->button_width * 2.5,
                              scale->button_height / 1.5},
                 GuiIconText(ICON_AUDIO, "Volume "),
                 TextFormat("%d %%", (int32_t) (config->volume * 100)), &config->volume, 0, 1);

    if (GuiButton((Rectangle) {scale->button_x / 3,
                               GetScreenHeight() - (GetScreenHeight() / 1.7),
                               scale->button_height * 1.5, scale->button_height * 1.5},
                  GuiIconText(ICON_BRUSH_PAINTER, ""))) {
        //  COLOR MENU
        is_theme = true, is_misc = false;
    }

    if (GuiButton((Rectangle) {scale->button_x / 3,
                               GetScreenHeight() - (GetScreenHeight() / 2.2),
                               scale->button_height * 1.5, scale->button_height * 1.5},
                  GuiIconText(ICON_GEAR_EX, ""))) {
        //  OTHER SETTINGS
        is_misc = true, is_theme = false;
    }

//    if (GuiButton((Rectangle) {scale->button_x / 3,
//                               GetScreenHeight() - (GetScreenHeight() / 3.1),
//                               scale->button_height * 1.5, scale->button_height * 1.5},
//                  GuiIconText(ICON_FILE_SAVE_CLASSIC, ""))) {
//        //  save settings?
//    }

    if (is_theme) {
        DrawText("Background Color:", scale->button_x - (scale->font_size * 4) + (scale->font_size / 2),
                 GetScreenHeight() / 2 - scale->font_size * 2, scale->font_size / 1.5, WHITE);

        GuiColorPicker((Rectangle) {scale->button_x - scale->button_width / 2, GetScreenHeight() / 2.2,
                                    scale->button_width / 1.5, scale->button_width / 1.5}, "BG Color",
                       &config->background_color);

        DrawText("Pixel Color:", scale->button_x * 1.85 - (scale->font_size * 4) + (scale->font_size / 2),
                 GetScreenHeight() / 2 - scale->font_size * 2, scale->font_size / 1.5, WHITE);

        GuiColorPicker((Rectangle) {scale->button_x + scale->button_width, GetScreenHeight() / 2.2,
                                    scale->button_width / 1.5, scale->button_width / 1.5}, "FG Color",
                       &config->pixel_color);
    }

    if (is_misc) {
        GuiCheckBox((Rectangle) {scale->button_x, GetScreenHeight() - (GetScreenHeight() / 1.7),
                                 scale->button_width / 8, scale->button_width / 8}, "Show Debug info",
                    &config->show_debug);

        GuiCheckBox((Rectangle) {scale->button_x, GetScreenHeight() - (GetScreenHeight() / 2.2),
                                 scale->button_width / 8, scale->button_width / 8}, "Show FPS", &config->show_fps);

        //  CUSTOM FONT LOADING?
//        if (GuiButton((Rectangle) {scale->button_x, GetScreenHeight() - (GetScreenHeight() / 3.2),
//                                   scale->button_width, scale->button_width / 8}, "Custom FONT binary")) {
//            ClearWindowState(FLAG_WINDOW_RESIZABLE);
//            fileDialogState = InitGuiWindowFileDialog(GetWorkingDirectory());
//            fileDialogState.windowActive = true;
//            config->is_dialog = true;
//        }
//
//        GuiUnlock();
//        GuiWindowFileDialog(&fileDialogState);

        if (config->show_debug == true) {
            SetTraceLogLevel(LOG_INFO);
        } else {
            SetTraceLogLevel(LOG_WARNING);
        }
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        SetExitKey(KEY_ESCAPE);
        return -1;
    }
    return 0;
}