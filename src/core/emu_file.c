
//==============================================================================
//    -  MIT License | emu_file.c
//
//      Copyright (c) 2023-2024, Tim <npxtune@scanf.dev>
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
        FILE *file = fopen(dropped_file.paths[0], "rb");

        if (file != NULL) {
            SetWindowTitle(GetFileName(*dropped_file.paths));
            TraceLog(LOG_INFO, "EMU_FILE -> Loading ROM into RAM");

            if ((fread(chip8->ram + 0x200, 1, RAM_SIZE, file)) > 0) {
                TraceLog(LOG_INFO, "EMU_FILE -> ROM was loaded into RAM");
                fclose(file);
                UnloadDroppedFiles(dropped_file);
                return 0;
            }
        }
        fclose(file);
    }
    UnloadDroppedFiles(dropped_file);
    TraceLog(LOG_ERROR, "EMU_FILE -> Invalid file!");
    return -1;
}