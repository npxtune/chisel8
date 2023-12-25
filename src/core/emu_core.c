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
            if (emu->opcode == 0xe0) {
//                printf("\nEMU_CORE: Fetched: %x\n", emu->opcode);
//                printf("EMU_CORE: CLEAR SCREEN\n");
                for (int x = 0; x < DISPLAY_WIDTH; ++x) {
                    for (int y = 0; y < DISPLAY_HEIGHT; ++y) {
                        emu->pixels[x][y] = 0;
                    }
                }
            } else if (emu->opcode == 0xee) {
                emu->pc = emu->stack[emu->i_stack];
                emu->stack[emu->i_stack] = 0;
                emu->i_stack -= 1;
            }
            break;
        case (0x1):
            //printf("jump\n\tPC->%03x\n", N3);
            emu->pc = N3;
            break;
        case (0x2):
//            printf("\nEMU_CORE: Fetched: %x\n", emu->opcode);
            emu->stack[emu->i_stack] = emu->pc;
            emu->i_stack += 1;
            emu-> pc = N3;
            break;
        case (0x3):
//            printf("\nEMU_CORE: Fetched: %x\n", emu->opcode);
            if (emu->reg[X] == N2) {
                emu->pc+=2;
            }
            break;
        case (0x4):
//            printf("\nEMU_CORE: Fetched: %x\n", emu->opcode);
            if (emu->reg[X] != N2) {
                emu->pc+=2;
            }
            break;
        case (0x5):
//            printf("\nEMU_CORE: Fetched: %x\n", emu->opcode);
            if(emu->reg[X] == emu->reg[Y]) {
                emu->pc+=2;
            }
            break;
        case (0x6):
//            printf("\nEMU_CORE: Fetched: %x\n", emu->opcode);
//            printf("EMU_CORE: set register VX\n\tX->%x\n\tVal->%02x\n", X , N2);
            emu->reg[X] = N2;
            break;
        case (0x7):
//            printf("\nEMU_CORE: Fetched: %x\n", emu->opcode);
//            printf("EMU_CORE: add value to register VX\n\tX->%x\n\tVal->%02x\n", X, N2);
            emu->reg[X] += N2;
            break;
            
        case (0x8):
            switch (N1) {
                case (0x0):
                    emu->reg[X] = emu->reg[Y];
                    break;
                case (0x1):
                    emu->reg[X] = (emu->reg[X] | emu-> reg[Y]);
                    break;
                case (0x2):
                    emu->reg[X] = (emu->reg[X] & emu-> reg[Y]);
                    break;
                case (0x3):
                    emu->reg[X] = (emu->reg[X] ^ emu-> reg[Y]);
                    break;
                case (0x4):
                    emu->reg[0xF] = 0;
                    if (emu->reg[X] + emu->reg[Y] > 255) {
                        emu->reg[0xF] = 1;
                    }
                    emu->reg[X] = emu->reg[X] + emu->reg[Y];
                    break;
                case (0x5):
                    emu->reg[0xF] = 1;
                    if (emu->reg[X] - emu->reg[Y] < 0) {
                        emu->reg[0xF] = 0;
                    }
                    emu->reg[X] = emu->reg[X] - emu->reg[Y];
                    break;
                case (0x7):
                    emu->reg[0xF] = 1;
                    if (emu->reg[Y] - emu->reg[X] < 0) {
                        emu->reg[0xF] = 0;
                    }
                    emu->reg[X] = emu->reg[Y] - emu->reg[X];
                    break;
                case (0x8):
                    emu->reg[0xF] = 0;
                    emu->reg[X] = emu->reg[Y];
                    if ((emu->reg[X] & 1) == 1) {
                        emu->reg[0xF] = 1;
                    }
                    emu->reg[X] >>= 1;
                    break;
                case (0xe):
                    emu->reg[0xF] = 0;
                    emu->reg[X] = emu->reg[Y];
                    if ((emu->reg[X] & 1) == 1) {
                        emu->reg[0xF] = 1;
                    }
                    emu->reg[X] <<= 1;
                    break;

                default:
                    fprintf(stderr, "%s", "EMU_CORE: FATAL ERROR -> COULD NOT DECODE INSTRUCTION!\n");
                    return -1;
            }
            break;
            
        case (0x9):
//            printf("\nEMU_CORE: Fetched: %x\n", emu->opcode);
            if(emu->reg[X] != emu->reg[Y]) {
                emu->pc+=2;
            }
            break;
        case (0xa):
//            printf("\nEMU_CORE: Fetched: %x\n", emu->opcode);
//            printf("EMU_CORE: set index register I\n\tI->%03x\n", N3);
            emu->I = N3;
            break;
        case (0xb):
            emu->pc = N3 + emu->reg[0x0];
            break;
        case (0xc):
            emu->reg[X] = GetRandomValue(0, 255) & N2;
            break;

        case (0xd):
//            printf("\nEMU_CORE: Fetched: %x\n", emu->opcode);
//            printf("EMU_CORE: display/draw\n\tX->%x\n\tY->%x\n\tN1->%x\n", X, Y, N1);
            0;
            uint8_t vx = emu->reg[X];
            uint8_t vy = emu->reg[Y];

            emu->reg[0xF] = 0;

            for (int y = 0; y < N1; ++y) {
                uint8_t pixel = emu->ram[emu->I + y];
                for (int x = 0; x < 8; ++x) {
                    if ((pixel & (0x80 >> x)) != 0) {
                        if (emu->pixels[vx + x][vy + y] == 1) {
                            emu->reg[0xF] = 1;
                        }
                        emu->pixels[vx + x][vy + y] ^= 1;
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