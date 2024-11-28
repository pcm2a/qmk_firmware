#include "sleep.h"

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"

#include "hardware/clocks.h"
#include "hardware/structs/scb.h"
#include "hardware/sync.h" 
#include "hardware/rtc.h"
#include "hardware/vreg.h"

// Go to sleep until woken up by the RTC
static void sleep_goto_sleep_until(datetime_t *t, rtc_callback_t callback) {
    clocks_hw->sleep_en0 = CLOCKS_SLEEP_EN0_CLK_RTC_RTC_BITS;
    clocks_hw->sleep_en1 = 0x0;

    rtc_set_alarm(t, callback);

    uint save = scb_hw->scr;
    scb_hw->scr = save | M0PLUS_SCR_SLEEPDEEP_BITS;

    __wfi();
}

void deep_sleep(void) {
    datetime_t t = {
            .year  = 2023,
            .month = 1,
            .day   = 1,
            .dotw  = 0, 
            .hour  = 0,
            .min   = 0,
            .sec   = 0
    };
    rtc_init();
    rtc_set_datetime(&t);
    t.sec = LOOP_DELAY_S;

    sleep_goto_sleep_until(&t, NULL);
}

void shutdown(void) {
    DBG_PRINTF("Entering shutdown!\n");
    hw_write_masked(&vreg_and_chip_reset_hw->bod, 0, VREG_AND_CHIP_RESET_BOD_BITS);
    hw_write_masked(&vreg_and_chip_reset_hw->vreg, 0, VREG_AND_CHIP_RESET_VREG_EN_BITS);
}
