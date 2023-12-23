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
    if (gui_load_file(&system) == -1) { // LOAD DROPPED FILE, IF NOT A ROM RETURN
        printf("EMU_MAIN: Emulation stopped\n");
        printf("===================================================\n");
        return -1;
    }
    for (int i = 0; i < FONT_SIZE; ++i) {
        system.ram[i + 0x050] = FONT[i];
    }
    printf("EMU_MAIN: Loaded FONT SET into ram\n");
    system.pc = 0x200;
    printf("EMU_MAIN: Set pc to Address 0x200 in ram\n");

    system.delay = 0, system.sound = 0;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        // Draw GRID for visual help
        for (int y = 0; y < DISPLAY_HEIGHT; ++y) {
            for (int x = 0; x < DISPLAY_WIDTH; ++x) {
                DrawLine(x * DISPLAY_MULTIPLIER, y * DISPLAY_MULTIPLIER, x * DISPLAY_MULTIPLIER, y * DISPLAY_MULTIPLIER + DISPLAY_MULTIPLIER, DARKGRAY);
                DrawLine(x * DISPLAY_MULTIPLIER, y * DISPLAY_MULTIPLIER, x * DISPLAY_MULTIPLIER + DISPLAY_MULTIPLIER, y * DISPLAY_MULTIPLIER, DARKGRAY);
            }
        }
        DrawText(TextFormat("FPS: %d", GetFPS()),10, 5, 20, WHITE);

        // MAIN EMU LOOP
        if(IsKeyPressed(KEY_SPACE)) {
            uint16_t instruction = fetch(&system);
            if (decode_exec(instruction, &system) == -1) {
                return -1;
            }
        }
        EndDrawing();
    }
    printf("EMU_MAIN: Emulation stopped\n");
    printf("===================================================\n");
    return 0;
}