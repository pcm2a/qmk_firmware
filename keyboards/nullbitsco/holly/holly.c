// Copyright 2022 Jay Greco
// SPDX-License-Identifier: GPL-2.0-or-later

#include "holly.h"
#include "flash.h"
#include "usb_util.h"
#include "analog.h"

static void disable_interrupts(void)
{
    SysTick_CTRL &= ~1;

    NVIC_ICER = 0xFFFFFFFF;
    NVIC_ICPR = 0xFFFFFFFF;
}

static void reset_peripherals(void)
{
    RESET = ~(
        RESETS_RESET_IO_QSPI_BITS |
        RESETS_RESET_PADS_QSPI_BITS |
        RESETS_RESET_SYSCFG_BITS |
        RESETS_RESET_PLL_SYS_BITS
    );
}

static void jump_to_vtor(uint32_t vtor)
{
    uint32_t reset_vector = *(volatile uint32_t *)(vtor + 0x04);

    SCB_VTOR = (volatile uint32_t)(vtor);

    asm volatile("msr msp, %0"::"g" (*(volatile uint32_t *)vtor));
    asm volatile("bx %0"::"r" (reset_vector));
}

void keyboard_post_init_kb(void) {
    #ifdef CONSOLE_ENABLE
    debug_enable = true;
    debug_matrix = true;
    #endif

    keyboard_post_init_user();

    #define SAMPLE_PERIODS 10
    #define MIN_READINGS 800

    int rolling_sum = 0;
    for (int i=0; i<SAMPLE_PERIODS; i++) {
        uint16_t reading = analogReadPin(VIN_PIN);
        rolling_sum += (int)reading;
        dprintf("adc reading: %u\n", reading);
        wait_ms(100);
    }
    
    rolling_sum /= SAMPLE_PERIODS;
    dprintf("final avg: %d\n", rolling_sum);

    // If either of the following are true, assume that we're running on battery power
    // Jump into the low-power optimized firmware, which will last much longer
    if (!usb_connected_state() || rolling_sum < MIN_READINGS) {
        dprintf("jumping to: %lu\n", (uint32_t)&bindata);
        wait_ms(100);
        disable_interrupts();
        reset_peripherals();
        jump_to_vtor((uint32_t)&bindata);
    } else {
        // Turn on the RGB LED power supplies
        setPinOutput(LED_VCC1);
        setPinOutput(LED_VCC2);
        writePinHigh(LED_VCC1);
        writePinHigh(LED_VCC2);
    }
}
