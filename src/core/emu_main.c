#include "core/emu_main.h"
#include "core/emu_definition.h"

int32_t emu_main(void) {
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

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        // Draw Pixels from virtual Texture
        DrawTextureEx(system.display, (Vector2){0, 0}, 0, (float)GetScreenHeight() / (float)system.display.height, WHITE);

        uint16_t instruction = fetch(&system);
        if (decode_exec(instruction, &system) == -1) {
            return -1;
        }
        EndDrawing();
    }
    UnloadTexture(system.display);
    printf("EMU_MAIN: Emulation stopped\n");
    printf("===================================================\n");
    return 0;
}