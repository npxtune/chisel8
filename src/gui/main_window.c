#include "gui/main_window.h"

//  RAYLIB INCLUDE
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION

#include "raygui.h"
#include "dark/style_dark.h"

#include "core/emu_main.h"
#include "core/emu_definition.h"
#include "gui/version.h"

// DEFINE GLOBAL VARIABLES
#define WINDOW_TITLE "chisel8-emu | version: "

void main_window(void) {
    const int32_t window_width = DISPLAY_WIDTH * DISPLAY_MULTIPLIER;
    const int32_t window_height = DISPLAY_HEIGHT * DISPLAY_MULTIPLIER;

    InitWindow(window_width,window_height,WINDOW_TITLE VERSION);
    GuiLoadStyleDark();

    enum menu_state_counter {normal, options, init};
    uint32_t menu_state = normal;

    SetTargetFPS(REFRESH_RATE);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        if (IsFileDropped()) {  // Initialize Emulation
            EndDrawing();
            if(emu_main() == -1) {break;}
            menu_state = normal;
        }

        SetWindowTitle(WINDOW_TITLE VERSION);

        switch (menu_state) {
            /*-------------------------------------------------------------------------------------------------------------*/
            case (normal):
                DrawText("Chisel8 Emulator", window_width/2-40*4, 75, 40, RAYWHITE);

                if (GuiButton((Rectangle){ (window_width+300)/3, window_height-400, 200, 30 }, GuiIconText(ICON_CPU, "Load EMU"))) {
                    menu_state = init;
                }

                if (GuiButton((Rectangle){ (window_width+300)/3, window_height-340, 200, 30 }, GuiIconText(ICON_GEAR, "Settings"))) {
                    menu_state = options;
                }

                if (GuiButton((Rectangle){ (window_width+300)/3, window_height-100, 200, 30 }, GuiIconText(ICON_EXIT, "Quit"))) {
                    EndDrawing();
                    CloseWindow();
                    return;
                }
                break;

            /*-------------------------------------------------------------------------------------------------------------*/

            case (options):
                // TODO: Call function from options_window source file and actually add options to choose from
                // TODO: Should allow the user to change the window scaling and other options...

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
                    break;
                }
                if (IsFileDropped()) {  // Initialize Emulation
                    EndDrawing();
                    if(emu_main() == -1) {break;}
                    menu_state = normal;
                    break;
                }
                break;

            /*-------------------------------------------------------------------------------------------------------------*/
        }
        EndDrawing();
    }
    CloseWindow();
}