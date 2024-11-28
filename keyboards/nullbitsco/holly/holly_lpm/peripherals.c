#include "colors.h"
#include "peripherals.h"

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"

#include "hardware/pio.h"
#include "hardware/vreg.h"
#include "ws2812.pio.h"

void configure_hardware(void) {
    // Lower core voltage
    #if !LIB_PICO_STDIO_USB
    vreg_set_voltage(VREG_VOLTAGE_1_00);
    #endif

    // Init switch GPIOs
    gpio_init(S1_PIN);
    gpio_init(S2_PIN);
    gpio_init(S3_PIN);
    gpio_init(S4_PIN);
    gpio_init(S5_PIN);
    gpio_init(S6_PIN);

    gpio_pull_up(S1_PIN);
    gpio_pull_up(S2_PIN);
    gpio_pull_up(S3_PIN);
    gpio_pull_up(S4_PIN);
    gpio_pull_up(S5_PIN);
    gpio_pull_up(S6_PIN);

    // Init PIO
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, false);
}

sw_t read_switches(void) {
    sw_t sw;

    sw.s1 = !gpio_get(S1_PIN);
    sw.s2 = !gpio_get(S2_PIN);
    sw.s3 = !gpio_get(S3_PIN);
    sw.s4 = !gpio_get(S4_PIN);
    sw.s5 = !gpio_get(S5_PIN);
    sw.s6 = !gpio_get(S6_PIN);

    return sw;
}

static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t) (r) << 8) | ((uint32_t) (g) << 16) | (uint32_t) (b);
}

static inline void enable_led_vcc(uint8_t index) {
    if (index == 1) {
        gpio_init(LED_VCC1_EN);
        gpio_set_dir(LED_VCC1_EN, GPIO_OUT);
        gpio_put(LED_VCC1_EN, 1);

    } else if (index == 2) {
        gpio_init(LED_VCC2_EN);
        gpio_set_dir(LED_VCC2_EN, GPIO_OUT);
        gpio_put(LED_VCC2_EN, 1);
    }
    sleep_ms(1);
}

static inline void disable_led_vcc(void) {
    gpio_init(LED_VCC1_EN);
    gpio_set_dir(LED_VCC1_EN, GPIO_IN);
    gpio_init(LED_VCC2_EN);
    gpio_set_dir(LED_VCC2_EN, GPIO_IN);
}

static void set_led(uint8_t led_index, uint8_t r,uint8_t g, uint8_t b) {
    // Write up to led_index with zero brightness
    for (int l=0; l<led_index; l++) {
        put_pixel(0);
    }
    
    // Write led_index with desired color
    put_pixel(urgb_u32(r, g, b));
}

static inline void fade_in(uint8_t l, uint8_t r, uint8_t g, uint8_t b)  {
    uint8_t ri=0, gi=0, bi=0;
    for (int s=0; s<FADE_STEPS; s++) {
        ri = (ri >= r) ? r : ++ri;
        gi = (gi >= g) ? g : ++gi;
        bi = (bi >= b) ? b : ++bi;
        set_led(l, ri, gi, bi);
        sleep_ms(FADE_DELAY);
    }
}

static inline void fade_out(uint8_t l, uint8_t r, uint8_t g, uint8_t b)  {
    uint8_t ri=r, gi=g, bi=b;
    for (int s=1; s<=FADE_STEPS; s++) {
        ri = (ri > 0) ? --ri : 0;
        gi = (gi > 0) ? --gi : 0;
        bi = (bi > 0) ? --bi : 0;
        set_led(l, ri, gi, bi);
        sleep_ms(FADE_DELAY);
    }
}

void clear_leds(void) {
    for (int l=0; l<NUM_LEDS; l++) {
        enable_led_vcc(1);
        if (l > 2) {
            enable_led_vcc(2);
        }

        set_led(l, 0, 0, 0);
    }
    disable_led_vcc();
}

// Snake animation with various colors
void led_snake(uint8_t mode) {
    DBG_PRINTF("led_snake mode %d\n", mode);

    uint8_t num_colors;
    if (mode == COLOR_MODE_SILVER) num_colors = NUM_SILVER_COLORS;
    else if (mode == COLOR_MODE_XMAS) num_colors = NUM_XMAS_COLORS;
    else num_colors = NUM_RGB_COLORS;
    uint8_t p = num_colors, c = num_colors;

    for (int l=0; l<NUM_LEDS; l++) {
        // Ensure we don't get the same color twice in a row
        while(c == p) {
            c = rand() % num_colors;
        }
        p = c;

        uint8_t r, g, b;
        if (mode == COLOR_MODE_SILVER) {
            r=silver_lut[c][R], g=silver_lut[c][G], b=silver_lut[c][B];
        } else if (mode == COLOR_MODE_XMAS) {
            r=xmas_lut[c][R], g=xmas_lut[c][G], b=xmas_lut[c][B];
        } else {
            r=rgb_lut[c][R], g=rgb_lut[c][G], b=rgb_lut[c][B];
        }

        enable_led_vcc(1);
        if (l > 2) {
            enable_led_vcc(2);
        }

        DBG_PRINTF("c/l (r/g/b): %d/%d (%d/%d/%d)\n", c, l, r, g, b);

        fade_in(l, r, g, b);
        sleep_ms(DWELL_DELAY);
        fade_out(l, r, g, b);

        disable_led_vcc();
        sleep_ms(CYCLE_DELAY);
    }
}

// Sparkle animation with various colors
void led_sparkle(uint8_t mode) {
    DBG_PRINTF("led_sparkle mode %d\n", mode);

    uint8_t num_colors;
    if (mode == COLOR_MODE_SILVER) num_colors = NUM_SILVER_COLORS;
    else if (mode == COLOR_MODE_XMAS) num_colors = NUM_XMAS_COLORS;
    else num_colors = NUM_RGB_COLORS;
    uint8_t p = num_colors, c = num_colors;

    uint8_t n = rand() % (MAX_SPARKLES + 1 - MIN_SPARKLES) + MIN_SPARKLES;

    for (int i=0; i<n; i++) {

        // Ensure we don't get the same color twice in a row
        while(c == p) {
            c = rand() % num_colors;
        }
        p = c;
        
        // Ensure we don't get the same LED twice in a row
        static uint8_t q = NUM_LEDS, l = NUM_LEDS;
        while(l == q) {
            l = rand() % NUM_LEDS;
        }
        q = l;

        uint8_t c = rand() % num_colors;

        uint8_t r, g, b;
        if (mode == COLOR_MODE_SILVER) {
            r=silver_lut[c][R], g=silver_lut[c][G], b=silver_lut[c][B];
        } else if (mode == COLOR_MODE_XMAS) {
            r=xmas_lut[c][R], g=xmas_lut[c][G], b=xmas_lut[c][B];
        } else {
            r=rgb_lut[c][R], g=rgb_lut[c][G], b=rgb_lut[c][B];
        }

        enable_led_vcc(1);
        if (l > 2) {
            enable_led_vcc(2);
        }

        DBG_PRINTF("n/c/l (r/g/b): %d/%d/%d (%d/%d/%d)\n", n, c, l, r, g, b);

        fade_in(l, r, g, b);
        sleep_ms(DWELL_DELAY);
        fade_out(l, r, g, b);

        disable_led_vcc();
        sleep_ms(CYCLE_DELAY);
    }
}

void led_test(void) {
    for (int l=0; l<NUM_LEDS; l++) {
        // Turn on LEDs
        enable_led_vcc(1);
        if (l > 2) {
            enable_led_vcc(2);
        }
        
        // Cycle through each RGB color on each LED
        for (int c=0; c<NUM_RGB_COLORS; c++) {
            uint8_t r=rgb_lut[c][R], g=rgb_lut[c][G], b=rgb_lut[c][B];
            DBG_PRINTF("c/l (r/g/b): %d/%d (%d/%d/%d)\n", c, l, r, g, b);
            fade_in(l, r, g, b);
            sleep_ms(10 * CYCLE_DELAY);
            fade_out(l, r, g, b);
        }
        sleep_ms(10 * CYCLE_DELAY);
        // Cycle through each xmas color on each LED
        for (int c=0; c<NUM_XMAS_COLORS; c++) {
            uint8_t r=xmas_lut[c][R], g=xmas_lut[c][G], b=xmas_lut[c][B];
            DBG_PRINTF("c/l (r/g/b): %d/%d (%d/%d/%d)\n", c, l, r, g, b);
            fade_in(l, r, g, b);
            sleep_ms(10 * CYCLE_DELAY);
            fade_out(l, r, g, b);
        }
        disable_led_vcc();
    }
}
