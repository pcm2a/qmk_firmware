#include "clocks.h"

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"

#include "hardware/clocks.h"
#include "hardware/pll.h"
#include "hardware/xosc.h"
#include "hardware/structs/rosc.h"

const uint32_t clocks_hw_wake_en0 =   ~(
    CLOCKS_WAKE_EN0_CLK_SYS_SPI1_BITS  |
    CLOCKS_WAKE_EN0_CLK_PERI_SPI1_BITS |
    CLOCKS_WAKE_EN0_CLK_SYS_SPI0_BITS  | 
    CLOCKS_WAKE_EN0_CLK_PERI_SPI0_BITS |
    CLOCKS_WAKE_EN0_CLK_SYS_PWM_BITS   |
    CLOCKS_WAKE_EN0_CLK_SYS_PIO1_BITS  |
    CLOCKS_WAKE_EN0_CLK_SYS_JTAG_BITS  |
    CLOCKS_WAKE_EN0_CLK_SYS_I2C1_BITS  |
    CLOCKS_WAKE_EN0_CLK_SYS_I2C0_BITS  |
    CLOCKS_WAKE_EN0_CLK_SYS_DMA_BITS   |
    CLOCKS_WAKE_EN0_CLK_SYS_ADC_BITS   |
    CLOCKS_WAKE_EN0_CLK_ADC_ADC_BITS   );

const uint32_t clocks_hw_wake_en1 =      ~(
    CLOCKS_WAKE_EN1_CLK_SYS_WATCHDOG_BITS |
    #if !LIB_PICO_STDIO_USB
    CLOCKS_WAKE_EN1_CLK_USB_USBCTRL_BITS  |
    CLOCKS_WAKE_EN1_CLK_SYS_USBCTRL_BITS  |
    #endif
    CLOCKS_WAKE_EN1_CLK_SYS_UART0_BITS    |
    CLOCKS_WAKE_EN1_CLK_PERI_UART0_BITS   );

const uint32_t clocks_hw_sleep_en0 =   ~(
    CLOCKS_SLEEP_EN0_CLK_SYS_SPI1_BITS  |
    CLOCKS_SLEEP_EN0_CLK_PERI_SPI1_BITS |
    CLOCKS_SLEEP_EN0_CLK_SYS_SPI0_BITS  | 
    CLOCKS_SLEEP_EN0_CLK_PERI_SPI0_BITS |
    CLOCKS_SLEEP_EN0_CLK_SYS_ROM_BITS   |
    CLOCKS_SLEEP_EN0_CLK_SYS_SIO_BITS   |
    CLOCKS_SLEEP_EN0_CLK_SYS_PWM_BITS   |
    CLOCKS_SLEEP_EN0_CLK_SYS_PSM_BITS   |
    CLOCKS_SLEEP_EN0_CLK_SYS_PIO1_BITS  |
    CLOCKS_SLEEP_EN0_CLK_SYS_PADS_BITS  |
    CLOCKS_SLEEP_EN0_CLK_SYS_JTAG_BITS  |
    CLOCKS_SLEEP_EN0_CLK_SYS_I2C1_BITS  |
    CLOCKS_SLEEP_EN0_CLK_SYS_I2C0_BITS  |
    CLOCKS_SLEEP_EN0_CLK_SYS_DMA_BITS   |
    CLOCKS_SLEEP_EN0_CLK_SYS_ADC_BITS   |
    CLOCKS_SLEEP_EN0_CLK_ADC_ADC_BITS   );

 const uint32_t clocks_hw_sleep_en1 =     ~(
    CLOCKS_SLEEP_EN1_CLK_SYS_XIP_BITS      |
    CLOCKS_SLEEP_EN1_CLK_SYS_WATCHDOG_BITS |
    #if !LIB_PICO_STDIO_USB
    CLOCKS_SLEEP_EN1_CLK_USB_USBCTRL_BITS  |
    CLOCKS_SLEEP_EN1_CLK_SYS_USBCTRL_BITS  |
    #endif
    CLOCKS_SLEEP_EN1_CLK_SYS_UART1_BITS    |
    CLOCKS_SLEEP_EN1_CLK_PERI_UART1_BITS   |
    CLOCKS_SLEEP_EN1_CLK_SYS_UART0_BITS    |
    CLOCKS_SLEEP_EN1_CLK_PERI_UART0_BITS   );

static inline void rosc_write(io_rw_32 *addr, uint32_t value) {
    hw_clear_bits(&rosc_hw->status, ROSC_STATUS_BADWRITE_BITS);
    *addr = value;
};

static void rosc_set_range(uint range) {
    rosc_write(&rosc_hw->ctrl, (ROSC_CTRL_ENABLE_VALUE_ENABLE << ROSC_CTRL_ENABLE_LSB) | range);
}

void measure_freqs(void) {
    uint f_pll_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_SYS_CLKSRC_PRIMARY);
    uint f_pll_usb = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_USB_CLKSRC_PRIMARY);
    uint f_rosc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_ROSC_CLKSRC_PH);
    uint f_xosc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_XOSC_CLKSRC);
    uint f_clk_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS);
    uint f_clk_peri = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_PERI);
    uint f_clk_usb = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_USB);
    uint f_clk_adc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_ADC);
    uint f_clk_rtc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_RTC);

    DBG_PRINTF("pll_sys  = %dkHz\n", f_pll_sys);
    DBG_PRINTF("pll_usb  = %dkHz\n", f_pll_usb);
    DBG_PRINTF("rosc     = %dkHz\n", f_rosc);
    DBG_PRINTF("xosc     = %dkHz\n", f_xosc);
    DBG_PRINTF("clk_sys  = %dkHz\n", f_clk_sys);
    DBG_PRINTF("clk_peri = %dkHz\n", f_clk_peri);
    DBG_PRINTF("clk_usb  = %dkHz\n", f_clk_usb);
    DBG_PRINTF("clk_adc  = %dkHz\n", f_clk_adc);
    DBG_PRINTF("clk_rtc  = %dkHz\n", f_clk_rtc);

    DBG_PRINTF("sysclk_get_hz: %d\n", clock_get_hz(clk_sys));
}

void configure_clocks(void) {
    #if LIB_PICO_STDIO_USB
    stdio_init_all();
    sleep_ms(1000);
    #endif

    // Bump up ROSC freq before switching over
    // rosc_set_range(ROSC_CTRL_FREQ_RANGE_VALUE_MEDIUM);
    uint f_rosc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_ROSC_CLKSRC);

    // CLK_REF = XOSC or ROSC
    clock_configure(clk_ref,
                    CLOCKS_CLK_REF_CTRL_SRC_VALUE_ROSC_CLKSRC_PH,
                    0, // No aux mux
                    f_rosc * KHZ,
                    f_rosc * KHZ);

    // CLK SYS = CLK_REF
    clock_configure(clk_sys,
                    CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLK_REF,
                    0, // Using glitchless mux
                    f_rosc * KHZ,
                    f_rosc * KHZ);

    #if !LIB_PICO_STDIO_USB
    clock_stop(clk_usb);
    #endif
    clock_stop(clk_adc);

    // CLK RTC = clk_sys
    clock_configure(clk_rtc,
                    0, // No GLMUX
                    CLOCKS_CLK_RTC_CTRL_AUXSRC_VALUE_ROSC_CLKSRC_PH,
                    f_rosc * KHZ,
                    46875);

    // CLK PERI = clk_sys. Used as reference clock for Peripherals. No dividers so just select and enable
    clock_configure(clk_peri,
                    0,
                    CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLK_SYS,
                    f_rosc * KHZ,
                    f_rosc * KHZ);

    pll_deinit(pll_sys);
    #if !LIB_PICO_STDIO_USB
    pll_deinit(pll_usb);
    xosc_disable();
    #else
    stdio_init_all();
    sleep_ms(1000);
    measure_freqs();
    #endif

    // Set default sleep and wake clocks
    clocks_hw->wake_en0 = clocks_hw_wake_en0;
    clocks_hw->wake_en1 = clocks_hw_wake_en1;

    clocks_hw->sleep_en0 = clocks_hw_sleep_en0;
    clocks_hw->sleep_en1 = clocks_hw_sleep_en1;
}

__force_inline void restore_clocks(void) {
    // Reset sleep-mode clocks back to default
    clocks_hw->sleep_en0 = clocks_hw_sleep_en0;
    clocks_hw->sleep_en1 = clocks_hw_sleep_en1;
}
