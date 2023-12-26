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
    emu->key = -1;
    if (IsKeyReleased(KEY_ONE)) {
        emu->key = 0x0;
    } else if (IsKeyReleased(KEY_TWO)) {
        emu->key = 0x1;
    } else if (IsKeyReleased(KEY_THREE)) {
        emu->key = 0x2;
    } else if (IsKeyReleased(KEY_FOUR)) {
        emu->key = 0x3;
    } else if (IsKeyReleased(KEY_Q)) {
        emu->key = 0x4;
    } else if (IsKeyReleased(KEY_W)) {
        emu->key = 0x5;
    } else if (IsKeyReleased(KEY_E)) {
        emu->key = 0x6;
    } else if (IsKeyReleased(KEY_R)) {
        emu->key = 0x7;
    } else if (IsKeyReleased(KEY_A)) {
        emu->key = 0x8;
    } else if (IsKeyReleased(KEY_S)) {
        emu->key = 0x9;
    } else if (IsKeyReleased(KEY_D)) {
        emu->key = 0xa;
    } else if (IsKeyReleased(KEY_F)) {
        emu->key = 0xb;
    } else if (IsKeyReleased(KEY_Z)) {
        emu->key = 0xc;
    } else if (IsKeyReleased(KEY_X)) {
        emu->key = 0xd;
    } else if (IsKeyReleased(KEY_C)) {
        emu->key = 0xe;
    } else if (IsKeyReleased(KEY_V)) {
        emu->key = 0xf;
    }
    //  This is horrible -> TODO: IMPROVE INPUT CHECKING!!!
}

int32_t emu_main(options_config *config) {
    chip8 system;
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

    system.delay = 0, system.sound = 0;

    system.key = false;

    system.display = LoadTextureFromImage(GenImageColor(DISPLAY_WIDTH, DISPLAY_HEIGHT, config->background_color));
    ClearWindowState(FLAG_WINDOW_RESIZABLE);

    while (!IsKeyPressed(KEY_ESCAPE)) {
        BeginDrawing();

        check_input(&system);

        // Draw Pixels from virtual Texture
        DrawTextureEx(system.display, (Vector2){0, 0}, 0, (float)config->display_scaling, WHITE);

        if (fetch(&system) == -1) {
            emu_stop(&system);
            return -1;
        }
        if (decode_exec(&system, config) == -1) {
            emu_stop(&system);
            return -1;
        }

        if(WindowShouldClose()) {
            emu_stop(&system);
            return -2;
        }
        system.delay -= 60, system.sound -= 60;
        EndDrawing();
    }
    emu_stop(&system);
    return 0;
}