#include "core/emu_main.h"
#include "core/emu_definition.h"

void emu_stop(chip8 *system) {
    EndDrawing();
    UnloadTexture(system->display);
    ClearBackground(BLACK);
    printf("EMU_MAIN: Stopping emulation\n");
    printf("===================================================\n");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
}

void check_input(chip8 *emu) {
    if (IsKeyPressed(KEY_ONE)) {
        emu->key = 0x1;
    } else if (IsKeyPressed(KEY_TWO)) {
        emu->key = 0x2;
    } else if (IsKeyPressed(KEY_THREE)) {
        emu->key = 0x3;
    } else if (IsKeyPressed(KEY_FOUR)) {
        emu->key = 0xc;
    } else if (IsKeyPressed(KEY_Q)) {
        emu->key = 0x4;
    } else if (IsKeyPressed(KEY_W)) {
        emu->key = 0x5;
    } else if (IsKeyPressed(KEY_E)) {
        emu->key = 0x6;
    } else if (IsKeyPressed(KEY_R)) {
        emu->key = 0x7;
    } else if (IsKeyPressed(KEY_A)) {
        emu->key = 0xa;
    } else if (IsKeyPressed(KEY_S)) {
        emu->key = 0x8;
    } else if (IsKeyPressed(KEY_D)) {
        emu->key = 0xd;
    } else if (IsKeyPressed(KEY_F)) {
        emu->key = 0xe;
    } else if (IsKeyPressed(KEY_Z)) {
        emu->key = 0x9;
    } else if (IsKeyPressed(KEY_X)) {
        emu->key = 0x0;
    } else if (IsKeyPressed(KEY_C)) {
        emu->key = 0xb;
    } else if (IsKeyPressed(KEY_V)) {
        emu->key = 0xf;
    } else {
        emu->key = -1;
    }
    //  This is horrible -> TODO: IMPROVE INPUT CHECKING!!!
}

int32_t emu_main(options_config *config) {
    chip8 system;
    bool undefined = false;

    printf("===================================================\n");
    printf("EMU_MAIN: Starting Emulation\n");
    for (int i = 0; i < RAM_SIZE; ++i) {
        system.ram[i] = 0;
    }
    printf("EMU_MAIN: Cleared ram\n");
    for (int i = 0; i < STACK_SIZE; ++i) {
        system.stack[i] = 0;
    }
    system.i_stack = -1;
    printf("EMU_MAIN: Cleared stack\n");
    for (int i = 0; i < REGISTER_SIZE; ++i) {
        system.reg[i] = 0;
    }
    printf("EMU_MAIN: Cleared registers\n");
    if (gui_load_file(&system) == -1) { // LOAD DROPPED FILE, IF NOT A ROM RETURN
        printf("EMU_MAIN: Emulation stopped\n");
        printf("===================================================\n");
        return -1;
    }
    for (int i = 0; i < FONT_SIZE; ++i) {
        system.ram[i] = FONT[i];
    }
    printf("EMU_MAIN: Loaded FONT SET into ram\n");
    system.pc = 0x200;
    printf("EMU_MAIN: Set pc to Address 0x200 in ram\n");

    for (int x = 0; x < DISPLAY_WIDTH; ++x) {
        for (int y = 0; y < DISPLAY_HEIGHT; ++y) {
            system.pixels[x][y] = 0;
        }
    }

    system.delay = 0, system.sound = 0;

    system.display = LoadTextureFromImage(GenImageColor(DISPLAY_WIDTH, DISPLAY_HEIGHT, config->background_color));
    ClearWindowState(FLAG_WINDOW_RESIZABLE);

    while (!IsKeyPressed(KEY_ESCAPE)) {
        BeginDrawing();

        // Draw Pixels from virtual Texture
        DrawTextureEx(system.display, (Vector2){0, 0}, 0, (float)config->display_scaling, WHITE);

        //DrawText(TextFormat("%dhz", GetFPS()), (int)config->display_scaling,(int)config->display_scaling/2, GetScreenHeight()/(config->display_scaling*1.5), DARKGREEN);

        check_input(&system);

        for (int i = 0; i < (int)(CLOCK_RATE/REFRESH_RATE); ++i) {
            if (!undefined && fetch(&system) == -1) {
                undefined = true;
                printf("EMU_MAIN: Pausing emulation!\n");
                continue;
            }
            if (!undefined && decode_exec(&system, config) == -1) {
                undefined = true;
                printf("EMU_MAIN: Pausing emulation!\n");
                continue;
            }
        }

        if(WindowShouldClose()) {
            emu_stop(&system);
            return -2;
        }
        system.delay -= REFRESH_RATE, system.sound -= REFRESH_RATE;
        EndDrawing();
    }
    emu_stop(&system);
    return 0;
}