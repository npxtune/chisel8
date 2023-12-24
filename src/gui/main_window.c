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

void main_window(options_config *config) {
    const int32_t window_width = DISPLAY_WIDTH * config->display_scaling;
    const int32_t window_height = DISPLAY_HEIGHT * config->display_scaling;

    InitWindow(window_width,window_height,WINDOW_TITLE VERSION);

    const float button_width = (float)(window_width/4.8);
    const float button_x = (float)((float)window_width/2-(window_width/9.6));
    GuiLoadStyleDark();

    enum menu_state_counter {normal, options, init};
    uint32_t menu_state = normal;

    SetTargetFPS(REFRESH_RATE);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        if (IsFileDropped()) {  // Initialize Emulation
            EndDrawing();
            if(emu_main(config) == -2) {break;}
            menu_state = normal;
        }

        SetWindowTitle(WINDOW_TITLE VERSION);

        switch (menu_state) {
            /*-------------------------------------------------------------------------------------------------------------*/
            case (normal):
                DrawText("Chisel8 Emulator", window_width/2-(75*40/20)-10, (window_height/12), 40, RAYWHITE);

                if (GuiButton((Rectangle){ button_x, window_height/2-window_height/6-10, button_width, 30 }, GuiIconText(ICON_CPU, "Load EMU"))) {
                    menu_state = init;
                }

                if (GuiButton((Rectangle){ button_x, window_height/2-window_height/16, button_width, 30 }, GuiIconText(ICON_GEAR, "Settings"))) {
                    menu_state = options;
                }

                if (GuiButton((Rectangle){ button_x, window_height-(window_height/6), button_width, 30 }, GuiIconText(ICON_EXIT, "Quit"))) {
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
                    if(emu_main(config) == -2) {break;}
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