#pragma once

// HOLLY defines
#define WS2812_PIN    8
#define LED_VCC1_EN   7
#define LED_VCC2_EN   6
#define NUM_SWITCHES  6
#define S1_PIN        27
#define S2_PIN        25
#define S3_PIN        23
#define S4_PIN        21
#define S5_PIN        19
#define S6_PIN        10

#define NUM_LEDS      6
#define MIN_SPARKLES  3
#define MAX_SPARKLES  5
#define FADE_STEPS    10

#define FADE_DELAY    8
#define DWELL_DELAY   10
#define CYCLE_DELAY   10
#define LOOP_DELAY_S  7
#define LOOP_DELAY_MS (LOOP_DELAY_S * 1000)

#if LIB_PICO_STDIO_USB
#define NUM_CYCLES    5 // Enter shutdown early for testing
#else
#define NUM_CYCLES    ((3600 * 3) / LOOP_DELAY_S) // 3 hours
#endif

#if LIB_PICO_STDIO_USB
#define DBG_PRINTF(...) printf(__VA_ARGS__)
#else
#define DBG_PRINTF(...) 
#endif

// On some samples, the xosc can take longer to stabilize than is usual
#ifndef PICO_XOSC_STARTUP_DELAY_MULTIPLIER
#define PICO_XOSC_STARTUP_DELAY_MULTIPLIER 64
#endif

//------------- UART -------------//
#ifndef PICO_DEFAULT_UART
#define PICO_DEFAULT_UART 1
#endif

#ifndef PICO_DEFAULT_UART_TX_PIN
#define PICO_DEFAULT_UART_TX_PIN 4
#endif

#ifndef PICO_DEFAULT_UART_RX_PIN
#define PICO_DEFAULT_UART_RX_PIN 5
#endif

//------------- LED -------------//
#ifndef PICO_DEFAULT_LED_PIN
#define PICO_DEFAULT_LED_PIN 3
#endif

#ifndef PICO_DEFAULT_WS2812_PIN
#define PICO_DEFAULT_WS2812_PIN 13
#endif

//------------- I2C -------------//
#ifndef PICO_DEFAULT_I2C
#define PICO_DEFAULT_I2C 1
#endif

#ifndef PICO_DEFAULT_I2C_SDA_PIN
#define PICO_DEFAULT_I2C_SDA_PIN 14
#endif

#ifndef PICO_DEFAULT_I2C_SCL_PIN
#define PICO_DEFAULT_I2C_SCL_PIN 15
#endif

//------------- SPI -------------//
#ifndef PICO_DEFAULT_SPI
#define PICO_DEFAULT_SPI 0
#endif

#ifndef PICO_DEFAULT_SPI_TX_PIN
#define PICO_DEFAULT_SPI_TX_PIN 23
#endif

#ifndef PICO_DEFAULT_SPI_RX_PIN
#define PICO_DEFAULT_SPI_RX_PIN 20
#endif

#ifndef PICO_DEFAULT_SPI_SCK_PIN
#define PICO_DEFAULT_SPI_SCK_PIN 22
#endif

//------------- FLASH -------------//

// Use slower generic flash access
#define PICO_BOOT_STAGE2_CHOOSE_GENERIC_03H 1

#ifndef PICO_FLASH_SPI_CLKDIV
#define PICO_FLASH_SPI_CLKDIV 2
#endif

#ifndef PICO_FLASH_SIZE_BYTES
#define PICO_FLASH_SIZE_BYTES (1 * 512 * 1024)
#endif

// All boards have B1 RP2040
#ifndef PICO_RP2040_B0_SUPPORTED
#define PICO_RP2040_B0_SUPPORTED 0
#endif
