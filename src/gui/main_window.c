#include "gui/main_window.h"
#include "core/emu_test.h"

//  RAYLIB INCLUDE
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION

#include "raygui.h"
#include "dark/style_dark.h"

// DEFINE GLOBAL VARIABLES
#define WINDOW_TITLE "chisel8-emu"
#define VERSION "  |  version: dev_main"

#define CHIP8_RES_WIDTH 32
#define CHIP8_RES_HEIGHT 64

int32_t main_window(void) {

    InitWindow(512, 256, WINDOW_TITLE VERSION);
    GuiLoadStyleDark();

    enum menu_state_counter {normal, options, init, debug};
    int32_t menu_state = normal;

    const int32_t window_width = (GetMonitorWidth(GetCurrentMonitor() / CHIP8_RES_WIDTH)/2);
    const int32_t window_height = (GetMonitorHeight(GetCurrentMonitor() / CHIP8_RES_HEIGHT)/2);

    SetWindowSize(window_width, window_height);
    SetWindowPosition(GetMonitorWidth(GetCurrentMonitor())/4, GetMonitorHeight(GetCurrentMonitor())/4);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawText(TextFormat("FPS: %d", GetFPS()),10, 5, 15, RAYWHITE);

        switch (menu_state) {
            case (normal):
                DrawText("Chisel8 Emulator", window_width/2-40*4, 75, 40, RAYWHITE);

                if (GuiButton((Rectangle){ (window_width+300)/3, window_height-400, 200, 30 }, GuiIconText(ICON_CPU, "Load EMU"))) {
                    menu_state = init;
                }

                if (GuiButton((Rectangle){ (window_width+300)/3, window_height-350, 200, 30 }, GuiIconText(ICON_TOOLS, "DEBUG EMU"))) {
                    menu_state = debug;
                }

                if (GuiButton((Rectangle){ (window_width+300)/3, window_height-100, 200, 30 }, GuiIconText(ICON_EXIT, "Quit"))) {
                    EndDrawing();
                    CloseWindow();
                    return 0;
                }
                break;

            case (options):
                break;

            case (init):
                DrawRectangle(0, 0, window_width, window_height, Fade(RAYWHITE, 0.2f));
                DrawText("Please drag a ROM file into the window", window_width-512*2-100, window_height-256*2+100, 50, RAYWHITE);

                if (GuiButton((Rectangle){ (window_width+300)/3, window_height-100, 200, 30 }, GuiIconText(ICON_REREDO_FILL, "Return"))) {
                    menu_state = normal;
                }
                break;

            case (debug):
                while (emu_test() != 0) {}
                menu_state = normal;
                break;

            default:
                CloseWindow();
                return -1;
                break;
        }

        EndDrawing();
    }
    CloseWindow();

    return 0;
}