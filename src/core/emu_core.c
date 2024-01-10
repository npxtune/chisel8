
//==============================================================================
//    -  MIT License | emu_core.c
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

#include "core/emu_core.h"

int32_t undefined(void) {           // Handles undefined behaviour
    TraceLog(LOG_ERROR, "EMU_CORE -> COULD NOT DECODE INSTRUCTION!");
    return -1;
}

int32_t fetch(emu *chip8) {      // Fetches instruction
    chip8->opcode = (uint8_t) chip8->ram[chip8->pc++];
    chip8->opcode <<= 8;
    chip8->opcode += chip8->ram[chip8->pc++];
    if (chip8->opcode == 0x0000) {
        return undefined();
    }
    return 0;
}

int32_t decode_exec(emu *chip8, options_config *config) {
    uint8_t X = (chip8->opcode >> 8) & 0xF;   // vX -> Register
    uint8_t Y = (chip8->opcode >> 4) & 0xF;   // vY -> Register
    uint8_t N1 = chip8->opcode & 0xF;         // Last nibble
    uint8_t N2 = chip8->opcode & 0xFF;        // 4th & 3rd nibble
    uint16_t N3 = chip8->opcode & 0xFFF;      // 4th, 3rd & 2nd nibble

    //----------------------------------------------------------------------------------------------------------------//

    switch (chip8->opcode >> 12) {            // Check first hex nibble & execute opcode accordingly
        case (0x0):
            if (chip8->opcode == 0x00e0) {
                for (int32_t x = 0; x < DISPLAY_WIDTH; ++x) {
                    for (int32_t y = 0; y < DISPLAY_HEIGHT; ++y) {
                        chip8->pixels[x][y] = 0;
                    }
                }

            } else if (chip8->opcode == 0x00ee) {
                chip8->pc = chip8->stack[chip8->i_stack];
                chip8->stack[chip8->i_stack] = 0;
                chip8->i_stack -= 1;

            } else {
                return undefined();
            }
            break;

        case (0x1):
            chip8->pc = N3;
            break;

        case (0x2):
            chip8->i_stack += 1;
            chip8->stack[chip8->i_stack] = chip8->pc;
            chip8->pc = N3;
            break;

        case (0x3):
            if (chip8->reg[X] == N2) {
                chip8->pc += 2;
            }
            break;

        case (0x4):
            if (chip8->reg[X] != N2) {
                chip8->pc += 2;
            }
            break;

        case (0x5):
            if (chip8->reg[X] == chip8->reg[Y]) {
                chip8->pc += 2;
            }
            break;

        case (0x9):
            if (chip8->reg[X] != chip8->reg[Y]) {
                chip8->pc += 2;
            }
            break;

        case (0x6):
            chip8->reg[X] = N2;
            break;

        case (0x7):
            chip8->reg[X] += N2;
            break;

        case (0x8):
            switch (N1) {

                case (0x0):
                    chip8->reg[X] = chip8->reg[Y];
                    break;

                case (0x1):
                    chip8->reg[X] |= chip8->reg[Y];
                    chip8->reg[0xf] = 0x0;
                    break;

                case (0x2):
                    chip8->reg[X] &= chip8->reg[Y];
                    chip8->reg[0xf] = 0x0;
                    break;

                case (0x3):
                    chip8->reg[X] ^= chip8->reg[Y];
                    chip8->reg[0xf] = 0x0;
                    break;

                case (0x4):
                    N1 = chip8->reg[0xf];
                    chip8->reg[X] += chip8->reg[Y];
                    if (N1 + chip8->reg[Y] > 255) {
                        chip8->reg[0xf] = 1;
                    } else {
                        chip8->reg[0xf] = 0;
                    }
                    break;

                case (0x5):
                    N1 = chip8->reg[X];
                    chip8->reg[X] -= chip8->reg[Y];
                    if (N1 < chip8->reg[Y]) {
                        chip8->reg[0xF] = 0;
                    } else {
                        chip8->reg[0xf] = 1;
                    }
                    break;

                case (0x7):
                    N1 = chip8->reg[X];
                    chip8->reg[X] = chip8->reg[Y] - chip8->reg[X];
                    if (chip8->reg[Y] < N1) {
                        chip8->reg[0xF] = 0;
                    } else {
                        chip8->reg[0xF] = 1;
                    }
                    break;

                case (0x6):
                    N1 = chip8->reg[Y];
                    chip8->reg[X] = chip8->reg[Y] >> 1;
                    if (N1 & 1) {
                        chip8->reg[0xf] = 1;
                    } else {
                        chip8->reg[0xf] = 0;
                    }
                    break;

                case (0xe):
                    N1 = chip8->reg[Y];
                    chip8->reg[X] = chip8->reg[Y] << 1;
                    if (N1 & 8) {
                        chip8->reg[0xf] = 1;
                    } else {
                        chip8->reg[0xf] = 0;
                    }
                    break;

                default:
                    return undefined();
            }
            break;

        case (0xa):
            chip8->I = N3 % RAM_SIZE;
            break;

        case (0xb):
            chip8->pc = N3 + chip8->reg[0x0];
            break;

        case (0xc):
            chip8->reg[X] = GetRandomValue(0, 255) & N2;
            break;

        case (0xe):

            if (N2 == 0x9e) {
                if (chip8->key == chip8->reg[X]) {
                    chip8->pc += 2;
                    chip8->key = -1;
                }

            } else if (N2 == 0xa1) {
                if (chip8->key != chip8->reg[X]) {
                    chip8->pc += 2;
                    chip8->key = -1;
                }

            } else {
                return undefined();
            }
            break;

        case (0xf):
            switch (N2) {

                case (0x07):
                    chip8->reg[X] = chip8->delay;
                    break;

                case (0x15):
                    chip8->delay = chip8->reg[X];
                    break;

                case (0x18):
                    chip8->sound = chip8->reg[X];
                    break;

                case (0x1e):
                    N1 = chip8->I;
                    chip8->I += chip8->reg[X];
                    if (N1 + chip8->reg[X] > 0x0FFF) {
                        chip8->reg[0xf] = 1;
                    } else {
                        chip8->reg[0xf] = 0;
                    }
                    break;

                case (0x0a):
                    if (chip8->key != -1) {
                        chip8->reg[X] = chip8->key;
                    } else {
                        chip8->pc -= 2;
                    }
                    break;

                case (0x29):
                    chip8->I = ((chip8->reg[X] & 0xf) * 5);
                    break;

                case (0x33):
                    N1 = chip8->reg[X];
                    chip8->ram[(chip8->I) % RAM_SIZE] = N1 / 100;
                    chip8->ram[(chip8->I + 1) % RAM_SIZE] = N1 % (((chip8->ram[(chip8->I) % RAM_SIZE]) * 100)) / 10;
                    chip8->ram[(chip8->I + 2) % RAM_SIZE] = N1 % (((chip8->ram[(chip8->I) % RAM_SIZE]) * 100 +
                                                                   (chip8->ram[(chip8->I + 1) % RAM_SIZE]) * 10));
                    break;

                case (0x55):
                    for (int32_t i = 0x0; i <= X; ++i) {
                        chip8->ram[(chip8->I + i) % RAM_SIZE] = chip8->reg[i];
                    }
                    break;

                case (0x65):
                    for (int32_t i = 0x0; i <= X; ++i) {
                        chip8->reg[i] = chip8->ram[(chip8->I + i) % RAM_SIZE];
                    }
                    break;

                default:
                    return undefined();
            }
            break;


        case (0xd):
            X = chip8->reg[X] % DISPLAY_WIDTH;
            Y = chip8->reg[Y] % DISPLAY_HEIGHT;

            chip8->reg[0xF] = 0;

            for (int32_t y = 0; y < N1; ++y) {
                uint8_t pixel = chip8->ram[chip8->I + y];
                for (int32_t x = 0; x < 8; ++x) {
                    if ((pixel & (0x80 >> x)) != 0) {
                        if (X + x < DISPLAY_WIDTH && Y + y < DISPLAY_HEIGHT) {
                            if (chip8->pixels[X + x][Y + y] == 1) {
                                chip8->reg[0xF] = 1;
                            }
                            chip8->pixels[X + x][Y + y] ^= 1;
                        }
                    }
                }
            }

            // Save pixels into a virtual Texture
            Image pixels = GenImageColor(DISPLAY_WIDTH, DISPLAY_HEIGHT, config->background_color);
            for (int32_t y = 0; y < DISPLAY_HEIGHT; ++y) {
                for (int32_t x = 0; x < DISPLAY_WIDTH; ++x) {
                    if (chip8->pixels[x][y] == 1) {
                        ImageDrawPixel(&pixels, x, y, config->pixel_color);
                    }
                }
            }
            UpdateTexture(chip8->display, pixels.data);
            UnloadImage(pixels);
            break;

        default:
            return undefined();
    }
    return 0;
}