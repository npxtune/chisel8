#include "core/emu_file.h"
#include "core/emu_definition.h"
#include <stdint.h>

uint16_t RAM[RAM_SIZE], PC = 0;

void drop_file() {
    if (IsFileDropped()) {
        FilePathList dropped_file = LoadDroppedFiles();
        if ((dropped_file.count == 1 && IsFileExtension(dropped_file.paths[0], ".ch8"))) {
            FILE *file = fopen(dropped_file.paths[0], "rb");
            if (file == NULL) {
                printf("Could not open file!\n");
                return;
            }
            size_t bytes;
            printf("INFO: Loading ROM into RAM\n");
            if ((bytes = fread(RAM, 1, sizeof(RAM), file)) > 0) {
                for (int32_t i = 0; i < bytes/2; i++) {
                    if (i % 8 == 0) {
                        printf("%04x\t", i == 0 ? 0 : i/8);
                    }
                    printf("%04x ", RAM[i]);
                    if ((i+1) % 8 == 0) {
                        printf("\n");
                    }
                }
                printf("\nINFO: ROM was loaded into RAM\n");
            } else {
                printf("INFO: Could not load ROM into RAM!\n");
            }
            fclose(file);
            printf("INFO: Unloaded file\n");
        } else {
            printf("INFO: Invalid file format, please load a valid Chip-8 ROM!\n");
        }
        UnloadDroppedFiles(dropped_file);
    }
}