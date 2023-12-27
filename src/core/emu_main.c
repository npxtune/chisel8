
//==============================================================================
//    -  MIT License | emu_main.c
//
//      Copyright (c) 2023, Tim <npxtune@scanf.dev>
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

#include "core/emu_main.h"

void emu_stop(emu *chip8) {
    EndDrawing();
    UnloadTexture(chip8->display);
    ClearBackground(BLACK);
    printf("EMU_MAIN: Stopping emulation\n");
    printf("===================================================\n");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
}

void check_input(emu *chip8) {
    if (IsKeyPressed(KEY_ONE)) {
        chip8->key = 0x1;
    } else if (IsKeyPressed(KEY_TWO)) {
        chip8->key = 0x2;
    } else if (IsKeyPressed(KEY_THREE)) {
        chip8->key = 0x3;
    } else if (IsKeyPressed(KEY_FOUR)) {
        chip8->key = 0xc;
    } else if (IsKeyPressed(KEY_Q)) {
        chip8->key = 0x4;
    } else if (IsKeyPressed(KEY_W)) {
        chip8->key = 0x5;
    } else if (IsKeyPressed(KEY_E)) {
        chip8->key = 0x6;
    } else if (IsKeyPressed(KEY_R)) {
        chip8->key = 0x7;
    } else if (IsKeyPressed(KEY_A)) {
        chip8->key = 0xa;
    } else if (IsKeyPressed(KEY_S)) {
        chip8->key = 0x8;
    } else if (IsKeyPressed(KEY_D)) {
        chip8->key = 0xd;
    } else if (IsKeyPressed(KEY_F)) {
        chip8->key = 0xe;
    } else if (IsKeyPressed(KEY_Z)) {
        chip8->key = 0x9;
    } else if (IsKeyPressed(KEY_X)) {
        chip8->key = 0x0;
    } else if (IsKeyPressed(KEY_C)) {
        chip8->key = 0xb;
    } else if (IsKeyPressed(KEY_V)) {
        chip8->key = 0xf;
    } else {
        chip8->key = -1;
    }
    //  This is horrible -> TODO: IMPROVE INPUT CHECKING!!!
}

int32_t emu_main(options_config *config) {
    emu chip8;
    bool undefined = false;

    printf("===================================================\n");
    printf("EMU_MAIN: Starting Emulation\n");
    for (int i = 0; i < RAM_SIZE; ++i) {
        chip8.ram[i] = 0;
    }
    printf("EMU_MAIN: Cleared ram\n");
    for (int i = 0; i < STACK_SIZE; ++i) {
        chip8.stack[i] = 0;
    }
    chip8.i_stack = -1;
    printf("EMU_MAIN: Cleared stack\n");
    for (int i = 0; i < REGISTER_SIZE; ++i) {
        chip8.reg[i] = 0;
    }
    printf("EMU_MAIN: Cleared registers\n");
    if (gui_load_file(&chip8) == -1) { // LOAD DROPPED FILE, IF NOT A ROM RETURN
        printf("EMU_MAIN: Emulation stopped\n");
        printf("===================================================\n");
        return -1;
    }
    for (int i = 0; i < FONT_SIZE; ++i) {
        chip8.ram[i] = FONT[i];
    }
    printf("EMU_MAIN: Loaded FONT SET into ram\n");
    chip8.pc = 0x200;
    printf("EMU_MAIN: Set pc to Address 0x200 in ram\n");

    for (int x = 0; x < DISPLAY_WIDTH; ++x) {
        for (int y = 0; y < DISPLAY_HEIGHT; ++y) {
            chip8.pixels[x][y] = 0;
        }
    }

    chip8.delay = 0, chip8.sound = 0;

    chip8.display = LoadTextureFromImage(GenImageColor(DISPLAY_WIDTH, DISPLAY_HEIGHT, config->background_color));
    ClearWindowState(FLAG_WINDOW_RESIZABLE);

    while (!IsKeyPressed(KEY_ESCAPE)) {
        BeginDrawing();

        // Draw Pixels from virtual Texture
        DrawTextureEx(chip8.display, (Vector2){0, 0}, 0, (float)config->display_scaling, WHITE);

        if(config->show_fps) {
            DrawText(TextFormat("%dhz", GetFPS()), (int)config->display_scaling,(int)config->display_scaling/2,
                     (int)(GetScreenHeight()/(config->display_scaling*1.5)), DARKGREEN);
        }

        check_input(&chip8);

        for (int i = 0; i < (int)(CLOCK_RATE/REFRESH_RATE); ++i) {
            if (!undefined && fetch(&chip8) == -1) {
                undefined = true;
                printf("EMU_MAIN: Pausing emulation!\n");
                continue;
            }
            if (!undefined && decode_exec(&chip8, config) == -1) {
                undefined = true;
                printf("EMU_MAIN: Pausing emulation!\n");
                continue;
            }
        }

        if(WindowShouldClose()) {
            emu_stop(&chip8);
            return -2;
        }
        chip8.delay -= REFRESH_RATE, chip8.sound -= REFRESH_RATE;
        EndDrawing();
    }
    emu_stop(&chip8);
    return 0;
}