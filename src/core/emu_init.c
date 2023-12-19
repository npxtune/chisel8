#include "core/emu_main.h"

uint8_t RAM[RAM_SIZE];
uint16_t STACK[STACK_SIZE];
uint8_t PC = 0x0;

int32_t emu_main(void) {
    printf("===================================================\n");
    printf("EMU_MAIN: Starting Emulation\n");
    for (int i = 0; i < RAM_SIZE; ++i) {
        RAM[i] = 0;
    }
    printf("EMU_MAIN: Cleared RAM\n");
    for (int i = 0; i < STACK_SIZE; ++i) {
        STACK[i] = 0;
    }
    printf("EMU_MAIN: Cleared STACK\n");
    gui_load_file(RAM);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        // Draw Pixels
        for (int y = 0; y < DISPLAY_HEIGHT; ++y) {
            for (int x = 0; x < DISPLAY_WIDTH; ++x) {
                DrawLine(x * DISPLAY_MULTIPLIER, y * DISPLAY_MULTIPLIER, x * DISPLAY_MULTIPLIER, y * DISPLAY_MULTIPLIER + DISPLAY_MULTIPLIER, DARKGRAY);
                DrawLine(x * DISPLAY_MULTIPLIER, y * DISPLAY_MULTIPLIER, x * DISPLAY_MULTIPLIER + DISPLAY_MULTIPLIER, y * DISPLAY_MULTIPLIER, DARKGRAY);
            }
        }
        DrawText(TextFormat("FPS: %d", GetFPS()),10, 5, 20, WHITE);

        EndDrawing();
    }
    printf("EMU_MAIN: Emulation stopped\n");
    printf("===================================================\n");
    return 0;
}