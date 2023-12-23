#include "raylib.h"
#include "core/emu_core.h"

uint16_t fetch(chip8 *system) {
    uint16_t instruction = (uint8_t) system->ram[system->pc++];
    instruction <<= 8;
    instruction += system->ram[system->pc++];
    printf("\nEMU_CORE: Fetched: %x\n", instruction);
    return instruction;
}

int32_t decode_exec(uint16_t instruction, chip8 *system) {
    uint8_t X = instruction & 0xF0;
    uint8_t Y = instruction & 0xF00;
    uint8_t N = instruction & 0xF;
    uint8_t NN = instruction & 0xFF;
    uint16_t NNN = instruction & 0xFFF;

    printf("EMU_CORE: *Decoding*\n");
    switch (instruction >> 12) {
        case (0x0):
            if (instruction == 0xe0) {
                printf("EMU_CORE: Decode -> CLEAR SCREEN\n");
                for (int i = 0; i < DISPLAY_HEIGHT; ++i) {
                    for (int j = 0; j < DISPLAY_WIDTH; ++j) {
                        system->pixels[j][i] = 0;
                    }
                }
            }
            break;
        case (0x1):
            printf("jump\n\tPC->%03x\n", NNN);
            system->pc = NNN;
            break;
        case (0x6):
            printf("set register VX\n\tX->%x\n\tVal->%02x\n", (instruction >> 8) & 0b01, NN);
            system->reg[X] = NN;
            break;
        case (0x7):
            printf("add value to register VX\n\tX->%x\n\tVal->%02x\n", (instruction >> 8) & 0b01, NN);
            system->reg[X] += NN;
            break;
        case (0xa):
            printf("set index register I\n\tI->%03x\n", NNN);
            system->I = NNN;
            break;
        case (0xd):
            printf("display/draw\n\tX->%x\n\tY->%x\n\tN->%x\n", X, Y, N);
            X = system->reg[X] % DISPLAY_WIDTH;
            Y = system->reg[Y] % DISPLAY_HEIGHT;
            system->reg[0xF] = 0;
            for (int i = 0; i < N; ++i) {

            }

            break;
        default:
            fprintf(stderr, "%s", "EMU_CORE: FATAL ERROR -> COULD NOT DECODE INSTRUCTION!\n");
            return -1;
    }
    return 0;
}