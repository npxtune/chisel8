#include "core/emu_core.h"

int32_t fetch(chip8 *system) {
    system->opcode = (uint8_t) system->ram[system->pc++];
    system->opcode <<= 8;
    system->opcode += system->ram[system->pc++];
    return 0;
}

int32_t decode_exec(chip8 *emu, options_config *config) {
    uint8_t X = (emu->opcode >> 8) & 0b01;
    uint8_t Y = (emu->opcode >> 4) & 0b01;
    uint8_t N1 = emu->opcode & 0xF;
    uint8_t N2 = emu->opcode & 0xFF;
    uint16_t N3 = emu->opcode & 0xFFF;

    switch (emu->opcode >> 12) {
        case (0x0):
            if (emu->opcode == 0x00e0) {
                for (int x = 0; x < DISPLAY_WIDTH; ++x) {
                    for (int y = 0; y < DISPLAY_HEIGHT; ++y) {
                        emu->pixels[x][y] = 0;
                    }
                }
            } else if (emu->opcode == 0x00ee) {
                emu->pc = emu->stack[emu->i_stack];
                emu->stack[emu->i_stack] = 0;
                emu->i_stack -= 1;
            } else {
                fprintf(stderr, "%s", "EMU_CORE: FATAL ERROR -> COULD NOT DECODE INSTRUCTION!\n");
                return -1;
            }
            break;

        case (0x1):
            emu->pc = N3;
            break;
        case (0x2):
            emu->i_stack += 1;
            emu->stack[emu->i_stack] = emu->pc;
            emu->pc = N3;
            break;
        case (0x3):
            if (emu->reg[X] == N2) {
                emu->pc+=2;
            }
            break;
        case (0x4):
            if (emu->reg[X] != N2) {
                emu->pc+=2;
            }
            break;
        case (0x5):
            if(emu->reg[X] == emu->reg[Y]) {
                emu->pc+=2;
            }
            break;
        case (0x6):
            emu->reg[X] = N2;
            break;
        case (0x7):
            emu->reg[X] += N2;
            break;
            
        case (0x8):
            switch (N1) {
                case (0x0):
                    emu->reg[X]=emu->reg[Y];
                    break;
                case (0x1):
                    emu->reg[X]|=emu-> reg[Y];
                    break;
                case (0x2):
                    emu->reg[X]&=emu-> reg[Y];
                    break;
                case (0x3):
                    emu->reg[X]^=emu-> reg[Y];
                    break;
                case (0x4):
                    emu->reg[0xf] = 0;
                    if (emu->reg[X] + emu->reg[Y] > 255) {
                        emu->reg[0xf] = 1;
                    }
                    emu->reg[X]+=emu->reg[Y];
                    break;
                case (0x5):
                    emu->reg[0xF] = 1;
                    if (emu->reg[X] - emu->reg[Y] < 0) {
                        emu->reg[0xF] = 0;
                    }
                    emu->reg[X]-=emu->reg[Y];
                    break;
                case (0x7):
                    emu->reg[0xF] = 1;
                    if (emu->reg[Y] - emu->reg[X] < 0) {
                        emu->reg[0xF] = 0;
                    }
                    emu->reg[X] = emu->reg[Y] - emu->reg[X];
                    break;
                case (0x6):
                    emu->reg[0xf] = 0;
                    emu->reg[X] = emu->reg[Y];
                    if ((emu->reg[X] & 1) == 1) {
                        emu->reg[0xf] = 1;
                    }
                    emu->reg[X] >>= 1;
                    break;
                case (0xe):
                    emu->reg[0xf] = 0;
                    emu->reg[X] = emu->reg[Y];
                    if ((emu->reg[X] & 1) == 1) {
                        emu->reg[0xf] = 1;
                    }
                    emu->reg[X] <<= 1;
                    break;

                default:
                    fprintf(stderr, "%s", "EMU_CORE: FATAL ERROR -> COULD NOT DECODE INSTRUCTION!\n");
                    return -1;
            }
            break;
            
        case (0x9):
            if(emu->reg[X] != emu->reg[Y]) {
                emu->pc+=2;
            }
            break;
        case (0xa):
            emu->I = N3;
            break;
        case (0xb):
            emu->pc = N3 + emu->reg[0x0];
            break;
        case (0xc):
            emu->reg[X] = GetRandomValue(0, 255) & N2;
            break;

        case (0xe):
            if (N2 == 0x9e) {
                if (emu->key == emu->reg[X]) {
                    emu->pc+=2;
                }
            } else if (N2 == 0xa1) {
                if (emu->key != emu->reg[X]) {
                    emu->pc+=2;
                }
            } else {
                fprintf(stderr, "%s", "EMU_CORE: FATAL ERROR -> COULD NOT DECODE INSTRUCTION!\n");
                return -1;
            }
            break;

        case (0xf):
            switch (N2) {
                case (0x07):
                    emu->reg[X] = emu->delay;
                    break;
                case (0x15):
                    emu->delay = emu->reg[X];
                    break;
                case (0x18):
                    emu->sound = emu->reg[X];
                    break;
                case (0x1e):
                    emu->reg[0xf] = 0;
                    if (emu->I+emu->reg[X] > 0x0FFF) {
                        emu->reg[0xf] = 1;
                    }
                    emu->I += emu->reg[X];
                    break;
                case (0x0a):
                    if (emu->key != -1) {
                        emu->reg[X] = emu->key;
                    } else {
                        emu->pc-=2;
                    }
                    break;
                case (0x29):
                    emu->I = (emu->ram[emu->reg[X&0xF]]);
                    break;
                case (0x33):
                    emu->ram[emu->I] = emu->reg[X] / 100;
                    emu->ram[emu->I+1] = emu->reg[X] % (((emu->I)*100) / 10);
                    emu->ram[emu->I+2] = emu->reg[X] % (((emu->I)*100) + ((emu->I+1)*10));
                    break;
                case (0x55):
                    emu->ram[emu->I] = emu->reg[0x0];
                    if (X != 0x0) {
                        for (int i = 0x1; i < 0xf; ++i) {
                            emu->ram[emu->I+i] = emu->reg[i];
                        }
                    }
                    break;
                case (0x65):
                    emu->reg[0x0] = emu->ram[emu->I];
                    if (X != 0x0) {
                        for (int i = 0x1; i < 0xf; ++i) {
                            emu->reg[i] = emu->ram[emu->I+i];
                        }
                    }
                    break;

                default:
                    printf("INSTRUCTION: %04x\n", emu->opcode);
                    fprintf(stderr, "%s", "EMU_CORE: FATAL ERROR -> COULD NOT DECODE INSTRUCTION!\n");
                    return -1;
            }
            break;


        case (0xd):
            X = emu->reg[X];
            Y = emu->reg[Y];

            emu->reg[0xF] = 0;

            for (int y = 0; y < N1; ++y) {
                uint8_t pixel = emu->ram[emu->I + y];
                for (int x = 0; x < 8; ++x) {
                    if ((pixel & (0x80 >> x)) != 0) {
                        if (X+x < DISPLAY_WIDTH && Y+y < DISPLAY_HEIGHT) {
                            if (emu->pixels[X + x][Y + y] == 1) {
                                emu->reg[0xF] = 1;
                            }
                            emu->pixels[X + x][Y + y] ^= 1;
                        } else {
                           printf("EMU_CORE: WARNING -> Tried to draw outside bounds:\tX: %03d | Y: %03d\n", X, Y);
                        }
                    }
                }
            }

            // Save pixels into a virtual Texture
            Image chip8_pixels = GenImageColor(DISPLAY_WIDTH, DISPLAY_HEIGHT, config->background_color);
            for (int y = 0; y < DISPLAY_HEIGHT; ++y) {
                for (int x = 0; x < DISPLAY_WIDTH; ++x) {
                    if (emu->pixels[x][y] == 1) {
                        ImageDrawPixel(&chip8_pixels, x, y, config->pixel_color);
                    }
                }
            }
            UnloadTexture(emu->display);
            emu->display = LoadTextureFromImage(chip8_pixels);
            UnloadImage(chip8_pixels);
            break;

        default:
            fprintf(stderr, "%s", "EMU_CORE: FATAL ERROR -> COULD NOT DECODE INSTRUCTION!\n");
            return -1;
    }
    return 0;
}