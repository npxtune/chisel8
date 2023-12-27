
//==============================================================================
//    -  MIT License | emu_file.c
//
//      Copyright (c) 2023, Tim <npxtune@scanf.dev>
//      All rights served.
//
//     Permission is hereby granted, free of charge, to any person obtaining a copy
//     of this software and associated documentation files (the "Software"), to deal
//     in the Software without restriction, including without limitation the rights
//     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//     copies of the Software, and to permit persons to whom the Software is
//     furnished to do so, subject to the following conditions:
//
//     The above copyright notice and this permission notice shall be included in all
//     copies or substantial portions of the Software.
//
//     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//     SOFTWARE.
//==============================================================================

#include "core/emu_file.h"

int32_t gui_load_file(emu *chip8) {
    FilePathList dropped_file = LoadDroppedFiles();
    if ((dropped_file.count == 1 && IsFileExtension(dropped_file.paths[0], ".ch8"))) {
        SetWindowTitle(GetFileName(*dropped_file.paths));
        FILE *file = fopen(dropped_file.paths[0], "rb");
        if (file == NULL) {
            printf("EMU_FILE: Could not open file!\n");
            return -1;
        }
        size_t bytes;
        printf("EMU_FILE: Loading ROM into ram:\n");
        if ((bytes = fread(chip8->ram+0x200, 1, RAM_SIZE, file)) > 0) {
            for (int32_t i = 0x200; i < bytes+0x200; i+=2) {
                if (i % 16 == 0) {
                    printf("0x%03x\t", i);
                }
                printf("%02x%02x ", chip8->ram[i], chip8->ram[i+1]);
                if ((i+2) % 16 == 0) {
                    printf("\n");
                }
            }
            printf("\nEMU_FILE: ROM was loaded into ram\n");
        } else {
            printf("EMU_FILE: Could not load ROM into ram!\n");
        }
        fclose(file);
        printf("EMU_FILE: Unloaded file\n");
    } else {
        printf("EMU_FILE: Invalid file format, please load a valid Chip-8 ROM!\n");
        UnloadDroppedFiles(dropped_file);
        return -1;
    }
    UnloadDroppedFiles(dropped_file);
    return 0;
}