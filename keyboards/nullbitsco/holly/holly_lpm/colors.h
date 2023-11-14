#pragma once

#include <stdint.h>

#define R 0
#define G 1
#define B 2

#define NUM_RGB_COLORS 31
const uint8_t rgb_lut[NUM_RGB_COLORS][3] = {
    {10, 0, 0},
    {10, 1, 0},
    {10, 4, 0},
    {10, 6, 0},
    {10, 8, 0},
    {10, 10, 0},
    {7, 10, 0},
    {6, 10, 0},
    {3, 10, 0},
    {2, 10, 0},
    {0, 10, 0},
    {0, 10, 1},
    {0, 10, 4},
    {0, 10, 6},
    {0, 10, 7},
    {0, 10, 10},
    {0, 7, 10},
    {0, 6, 10},
    {0, 4, 10},
    {0, 2, 10},
    {0, 0, 10},
    {1, 0, 10},
    {3, 0, 10},
    {6, 0, 10},
    {8, 0, 10},
    {10, 0, 10},
    {10, 0, 7},
    {10, 0, 5},
    {10, 0, 4},
    {10, 0, 2},
    {5, 5, 5},
};

#define NUM_XMAS_COLORS 2
const uint8_t xmas_lut[NUM_XMAS_COLORS][3] = {
    {10, 0, 0},
    {0, 10, 0},
};

#define NUM_SILVER_COLORS 3
const uint8_t silver_lut[NUM_SILVER_COLORS][3] = {
    {1, 1, 1},
    {5, 5, 5},
    {10, 10, 10},
};
