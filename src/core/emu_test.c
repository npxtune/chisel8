#include "core/emu_test.h"
#include "core/emu_definition.h"
#include "raylib.h"
#include <stdint.h>

int32_t emu_test(void) {
    while (!WindowShouldClose()) {
        BeginDrawing();
        DrawFPS(2,2);
        DrawText("EMU TEST", 100, 2, 20, RAYWHITE);
        ClearBackground(BLACK);
        EndDrawing();
    }
    return 0;
}