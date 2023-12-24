#include "core/emu_core.h"

uint16_t fetch(chip8 *system) {
    uint16_t instruction = (uint8_t) system->ram[system->pc++];
    instruction <<= 8;
    instruction += system->ram[system->pc++];
    return instruction;
}

int32_t decode_exec(uint16_t instruction, chip8 *system) {
    uint8_t X = (instruction >> 8) & 0b01;
    uint8_t Y =  (instruction >> 4) & 0b01;
    uint8_t N1 = instruction & 0xF;
    uint8_t N2 = instruction & 0xFF;
    uint16_t N3 = instruction & 0xFFF;

    switch (instruction >> 12) {
        case (0x0):
            if (instruction == 0xe0) {
                printf("\nEMU_CORE: Fetched: %x\n", instruction);
                printf("EMU_CORE: CLEAR SCREEN\n");
                for (int x = 0; x < DISPLAY_WIDTH; ++x) {
                    for (int y = 0; y < DISPLAY_HEIGHT; ++y) {
                        system->pixels[x][y] = 0;
                    }
                }
            }
            break;
        case (0x1):
            //printf("jump\n\tPC->%03x\n", N3);
            system->pc = N3;
            break;
        case (0x6):
            printf("\nEMU_CORE: Fetched: %x\n", instruction);
            printf("EMU_CORE: set register VX\n\tX->%x\n\tVal->%02x\n", X , N2);
            system->reg[X] = N2;
            break;
        case (0x7):
            printf("\nEMU_CORE: Fetched: %x\n", instruction);
            printf("EMU_CORE: add value to register VX\n\tX->%x\n\tVal->%02x\n", X, N2);
            system->reg[X] += N2;
            break;
        case (0xa):
            printf("\nEMU_CORE: Fetched: %x\n", instruction);
            printf("EMU_CORE: set index register I\n\tI->%03x\n", N3);
            system->I = N3;
            break;

        case (0xd):
            printf("\nEMU_CORE: Fetched: %x\n", instruction);
            printf("EMU_CORE: display/draw\n\tX->%x\n\tY->%x\n\tN1->%x\n", X, Y, N1);

            uint8_t vx = system->reg[X];
            uint8_t vy = system->reg[Y];

            system->reg[0xF] = 0;

            for (int y = 0; y < N1; ++y) {
                uint8_t pixel = system->ram[system->I + y];
                for (int x = 0; x < 8; ++x) {
                    if ((pixel & (0x80 >> x)) != 0) {
                        if (system->pixels[vx+x][vy+y] == 1) {
                            system->reg[0xF] = 1;
                        }
                        system->pixels[vx+x][vy+y] ^= 1;
                    }
                }
            }
            break;

        default:
            fprintf(stderr, "%s", "EMU_CORE: FATAL ERROR -> COULD NOT DECODE INSTRUCTION!\n");
            return -1;
    }
    return 0;
}