#include "core/emu_test.h"
#include "core/emu_definition.h"
#include "raylib.h"
#include <stdint.h>

int32_t emu_test(void) {
    while (!WindowShouldClose()) {
        BeginDrawing();
        DrawText(TextFormat("FPS: %d", GetFPS()),10, 5, 20, RAYWHITE);
        DrawText("| EMU TEST", 100, 5, 20, RAYWHITE);
        ClearBackground(BLACK);
        EndDrawing();
    }
    return 0;
}