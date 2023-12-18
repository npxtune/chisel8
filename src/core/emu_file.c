#include "core/emu_file.h"

void gui_load_file(uint8_t *RAM) {
    FilePathList dropped_file = LoadDroppedFiles();
    if ((dropped_file.count == 1 && IsFileExtension(dropped_file.paths[0], ".ch8"))) {
        FILE *file = fopen(dropped_file.paths[0], "rb");
        if (file == NULL) {
            printf("EMU_FILE: Could not open file!\n");
            return;
        }
        size_t bytes;
        printf("EMU_FILE: Loading ROM into RAM\n");
        if ((bytes = fread(RAM, 1, RAM_SIZE, file)) > 0) {
            for (int32_t i = 0; i < bytes; i+=2) {
                if (i % 16 == 0) {
                    printf("%04x\t", i == 0 ? 0 : i/16);
                }
                printf("%02x%02x ", RAM[i], RAM[i+1]);
                if ((i+2) % 16 == 0) {
                    printf("\n");
                }
            }
            printf("\nEMU_FILE: ROM was loaded into RAM\n");
        } else {
            printf("EMU_FILE: Could not load ROM into RAM!\n");
        }
        fclose(file);
        printf("EMU_FILE: Unloaded file\n");
    } else {
        printf("EMU_FILE: Invalid file format, please load a valid Chip-8 ROM!\n");
    }
    UnloadDroppedFiles(dropped_file);
}