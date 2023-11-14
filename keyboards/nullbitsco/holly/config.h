/*
Copyright 2021 Jay Greco

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/* NOTE: This config file is specific to RP2040 builds. */

#pragma once

#define RGBLIGHT_DEFAULT_MODE RGBLIGHT_MODE_CHRISTMAS

/* RP2040-specific defines*/
#define PICO_XOSC_STARTUP_DELAY_MULTIPLIER 64
#define RP2040_FLASH_GENERIC_03H
