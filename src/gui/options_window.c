#include "gui/options_window.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

//  DEFAULT SETTINGS VALUES

#define color_0     "  0,  0,  0, 1;    # Background color\n"       // Default color is BLACK
#define color_1     "255,255,255, 1;    # Pixel color\n"            // Default color is WHITE
#define scale       "15;                # Display scaling, Chip8 Display -> 32x64 Pixels\n"
#define debug       "false;             # Show debugging info in terminal\n"
#define fps         "false;             # Show Frames per Second in the top left\n"

#define items 5     // How many options are configurable?

void create_config(options_config *config) {
    FILE *file = fopen("./chisel8-settings.txt", "w");

    fwrite(color_0, strlen(color_0), 1, file);
    fwrite(color_1, strlen(color_1), 1, file);
    fwrite(scale, strlen(scale), 1, file);
    fwrite(debug, strlen(debug), 1, file);
    fwrite(fps, strlen(fps), 1, file);

    config->background_color = BLACK;
    config->pixel_color = WHITE;
    config->display_scaling = 15;
    config->show_debug = false;
    config->show_fps = false;
    config->main_volume = 0;

    fclose(file);
}

void load_settings(options_config *config) {
    FILE *file = fopen("./chisel8-settings.txt", "r");
    if (file != NULL) {
        // READ OPTIONS
        char line[250];
        for (int i = 0; i < items; ++i) {
            fgets(line, 250, file);
            uint16_t color[4] = {0,0,0,0};
            uint32_t counter = 0;

            // Skip line if it is not a value
            if (line[0] == '#' || line[0] == '\n' || line[0] == ' ') {
                i -= 1;
                continue;
            }

            switch (i) {
                case 0:
                    for (int j = 0; line[j] != ';'; ++j) {
                        if (isnumber(line[j]) && color[counter] > 0) {
                            color[counter] = (color[counter] * 10) + line[j] - '0';
                        } else if (isnumber(line[j])) {
                            color[counter] = line[j] - '0';
                        }
                        if (line[j] == ',') {
                            counter++;
                        }
                    }
                    config->background_color = (Color) {color[0], color[1], color[2], color[3] * 255};
                    break;

                case 1:
                    for (int j = 0; line[j] != ';'; ++j) {
                        if (isnumber(line[j]) && color[counter] > 0) {
                            color[counter] = (color[counter] * 10) + line[j] - '0';
                        } else if (isnumber(line[j])) {
                            color[counter] = line[j] - '0';
                        }
                        if (line[j] == ',') {
                            counter++;
                        }
                    }
                    config->pixel_color = (Color) {color[0], color[1], color[2], color[3] * 255};
                    break;

                case 2:
                    for (int j = 0; line[j] != ';'; ++j) {
                        if (isnumber(line[j]) && color[0] > 0) {
                            color[0] = (color[0] * 10) + line[j] - '0';
                        } else if (isnumber(line[j])) {
                            color[0] = line[j] - '0';
                        }
                    }
                    config->display_scaling = color[0];
                    break;

//                case 3:
//                    break;
//
//                case 4:
//                    break;

                default:
                    fclose(file);
                    return;
            }
        }
    } else {
        // CREATE OPTIONS
        create_config(config);
    }
    fclose(file);
}