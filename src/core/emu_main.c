#include "core/emu_main.h"
#include "core/emu_definition.h"

void emu_stop(chip8 *system) {
    EndDrawing();
    UnloadTexture(system->display);
    ClearBackground(BLACK);
    printf("EMU_MAIN: Stopping emulation\n");
    printf("===================================================\n");
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

    while (!IsKeyPressed(KEY_ESCAPE)) {
        BeginDrawing();
        ClearBackground(BLACK);

        // Draw Pixels from virtual Texture
        DrawTextureEx(system.display, (Vector2){0, 0}, 0, (float)GetScreenHeight() / (float)system.display.height, WHITE);

        uint16_t instruction = fetch(&system);
        if (decode_exec(instruction, &system, config) == -1) {
            emu_stop(&system);
            return -1;
        }

        if(WindowShouldClose()) {
            emu_stop(&system);
            return -2;
        }
        EndDrawing();
    }
    emu_stop(&system);
    return 0;
}