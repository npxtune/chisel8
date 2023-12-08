#include "gui/main_window.h"
#include "core/emu_test.h"
#include "gui/version.h"

//  RAYLIB INCLUDE
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION

#include "raygui.h"
#include "dark/style_dark.h"

// DEFINE GLOBAL VARIABLES
#define WINDOW_TITLE "chisel8-emu | version: "

#define CHIP8_RES_WIDTH 64
#define CHIP8_RES_HEIGHT 32

#define WINDOW_MULTIPLIER 20        // 1440 / 72 = 20

int32_t main_window(void) {
    const int32_t window_width = CHIP8_RES_WIDTH*WINDOW_MULTIPLIER;
    const int32_t window_height = CHIP8_RES_HEIGHT*WINDOW_MULTIPLIER;

    InitWindow(window_width,window_height,WINDOW_TITLE VERSION);
    GuiLoadStyleDark();

    enum menu_state_counter {normal, options, init, debug};
    uint32_t menu_state = normal;

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawText(TextFormat("FPS: %d", GetFPS()),10, 5, 20, RAYWHITE);

        switch (menu_state) {
            /*-------------------------------------------------------------------------------------------------------------*/
            case (normal):
                DrawText("Chisel8 Emulator", window_width/2-40*4, 75, 40, RAYWHITE);

                if (GuiButton((Rectangle){ (window_width+300)/3, window_height-400, 200, 30 }, GuiIconText(ICON_CPU, "Load EMU"))) {
                    menu_state = init;
                }

                if (GuiButton((Rectangle){ (window_width+300)/3, window_height-350, 200, 30 }, GuiIconText(ICON_TOOLS, "DEBUG EMU"))) {
                    menu_state = debug;
                }

                if (GuiButton((Rectangle){ (window_width+300)/3, window_height-300, 200, 30 }, GuiIconText(ICON_GEAR, "Settings"))) {
                    menu_state = options;
                }

                if (GuiButton((Rectangle){ (window_width+300)/3, window_height-100, 200, 30 }, GuiIconText(ICON_EXIT, "Quit"))) {
                    EndDrawing();
                    CloseWindow();
                    return 0;
                }
                break;

            /*-------------------------------------------------------------------------------------------------------------*/

            case (options):
                // TODO: Call function from options_window source file and actually add options to choose from

                DrawText("TODO...", window_width-512*2+300, window_height-256*2+100, 50, RAYWHITE);
                if (GuiButton((Rectangle){ (window_width+300)/3, window_height-100, 200, 30 }, GuiIconText(ICON_REREDO_FILL, "Return"))) {
                    menu_state = normal;
                }
                break;

            /*-------------------------------------------------------------------------------------------------------------*/

            case (init):
                DrawRectangle(0, 0, window_width, window_height, Fade(RAYWHITE, 0.2f));
                DrawText("Please drag a ROM file into the window", window_width-512*2-100, window_height-256*2+100, 50, RAYWHITE);

                if (GuiButton((Rectangle){ (window_width+300)/3, window_height-100, 200, 30 }, GuiIconText(ICON_REREDO_FILL, "Return"))) {
                    menu_state = normal;
                }
                break;

            /*-------------------------------------------------------------------------------------------------------------*/

            case (debug):
                while (emu_test() != 0) {
                    EndDrawing();
                }
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