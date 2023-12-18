#include "core/emu_init.h"

uint8_t RAM[RAM_SIZE];

int32_t emu_init(void) {
    for (int i = 0; i < RAM_SIZE; ++i) {
        RAM[i] = 0;
    }
    gui_load_file(RAM);

    while (!WindowShouldClose()) {
        BeginDrawing();
        DrawText(TextFormat("FPS: %d", GetFPS()),10, 5, 20, RAYWHITE);
        DrawText("| ROM Mode", 100, 5, 20, RAYWHITE);
        ClearBackground(BLACK);
        EndDrawing();
    }
    for (int i = 0; i < RAM_SIZE; ++i) {
        RAM[i] = 0;
    }
    printf("EMU_INIT: Cleared RAM\n");
    printf("EMU_INIT: Emulation stopped\n");
    return 0;
}