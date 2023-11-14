#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"

#include "clocks.h"
#include "peripherals.h"
#include "sleep.h"

int main() {
    configure_clocks();
    configure_hardware();

    #if LIB_PICO_STDIO_USB
    #pragma message "USB is enabled. Deep sleep is disabled!!"
    #endif

    // Read switch states on boot
    sw_t sw_state = read_switches();
    
    // Track number of cycles
    uint16_t num = 0;

    while (true) {
        // Use switch state to choose color and animation
        if (sw_state.s6) led_snake(COLOR_MODE_SILVER);
        else if (sw_state.s5) led_snake(COLOR_MODE_XMAS);
        else if (sw_state.s4) led_snake(COLOR_MODE_RGB);
        else if (sw_state.s3) led_sparkle(COLOR_MODE_SILVER);
        else if (sw_state.s2) led_sparkle(COLOR_MODE_XMAS);
        else led_sparkle(COLOR_MODE_RGB);

        // Only enter deep sleep if USB is disabled
        #if !LIB_PICO_STDIO_USB
        deep_sleep();
        restore_clocks();
        #else
        sleep_ms(LOOP_DELAY_MS);
        #endif

        // Enter shutdown after NUM_CYCLES
        if (++num >= NUM_CYCLES) {
            shutdown();
        }
    }
}
