#pragma once

#include <stdint.h>

#define COLOR_MODE_RGB 0
#define COLOR_MODE_XMAS 1
#define COLOR_MODE_SILVER 2

typedef struct {
    uint8_t s1 : 1;
    uint8_t s2 : 1;
    uint8_t s3 : 1;
    uint8_t s4 : 1;
    uint8_t s5 : 1;
    uint8_t s6 : 1;
    uint8_t unused : 2;
} sw_t;

void configure_hardware(void);
sw_t read_switches(void);
void clear_leds(void);
void led_test(void);
void led_sparkle(uint8_t mode);
void led_snake(uint8_t mode);
