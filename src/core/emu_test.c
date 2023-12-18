#include "core/emu_test.h"
#include "core/emu_stack.h"
#include "raylib.h"
#include <stdio.h>

int32_t emu_test(void) {
    while (!WindowShouldClose()) {
        BeginDrawing();
        DrawText(TextFormat("FPS: %d", GetFPS()),10, 5, 20, RAYWHITE);
        DrawText("| EMU TEST", 100, 5, 20, RAYWHITE);

        // 700 instructions per second

        for (int32_t i = 0; i < 700; ++i) {

        }


        ClearBackground(BLACK);
        EndDrawing();
    }
    return 0;
}