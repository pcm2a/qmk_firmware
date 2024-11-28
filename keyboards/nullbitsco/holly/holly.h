// Copyright 2022 Jay Greco
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "quantum.h"
#include <hal.h>

#define LED_VCC1 GP7
#define LED_VCC2 GP6
#define VIN_PIN GP29

#define RESETS_RESET_IO_QSPI_BITS   0x00000040
#define RESETS_RESET_PADS_QSPI_BITS 0x00000200
#define RESETS_RESET_SYSCFG_BITS    0x00040000
#define RESETS_RESET_PLL_SYS_BITS   0x00001000

#define SysTick_CTRL (*(uint32_t *)(0xE0000000UL + 0xE010UL))
#define NVIC_ICER (*(uint32_t *)(0xE0000000UL + 0xE180UL))
#define NVIC_ICPR (*(uint32_t *)(0xE0000000UL + 0xE280UL))
#define RESET (*(uint32_t *)(0x4000C000UL + 0x2000UL))
#define SCB_VTOR (*(uint32_t *)(0xE0000000UL + 0xED08UL))
