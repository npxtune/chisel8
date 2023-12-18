
#pragma once
#include <stdint.h>
#include <stdbool.h>

#define MAXSIZE 16

typedef struct emu_stack {
    uint32_t maxsize;
    uint32_t top;
    uint16_t items[MAXSIZE];

} Stack;

Stack stack_create() {
    Stack stack = { MAXSIZE, 0};
    return stack;
}

void stack_push(Stack *stack, uint16_t item) {
    if (stack->top == stack->maxsize) {
        return;
    }
    stack->items[stack->top++] = item;
}

void stack_pop(Stack *stack) {
    if (stack->top == 0) {
        return;
    }
    stack->items[stack->top-1] = 0;
    stack->top-=1;
}

uint16_t stack_top(Stack stack) {
    if (stack.top != 0) {
        return stack.items[stack.top-1];
    }
    return stack.items[stack.top];
}

bool stack_empty(Stack stack) {
    if (stack.top == 0) {
        return true;
    }
    return false;
}