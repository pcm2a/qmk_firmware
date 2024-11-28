/* Copyright 2023 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "keychron_common.h"
#include <os_detection.h>

enum more_keycodes {
  FN_MAC = SAFE_RANGE,
  FN_WIN,
  RGB_TOGX,
  RGB_MODX,
  RGB_VAIX,
  RGB_SPIX,
  RGB_RMODX,
  RGB_VADX,
  RGB_SPDX,

  NUMX
};

enum layers{
  MAC_BASE,
  MAC_FN,
  WIN_BASE,
  WIN_FN,
  GAME,
  NP_NUM,
  NP_FUN,
};

const uint8_t PROGMEM RGB_GROUP_FN[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
const uint8_t PROGMEM RGB_GROUP_MOD[] = { 16, 33, 46, 50, 62, 63, 74, 76, 77, 78, 80, 81, 82, 83};
const uint8_t PROGMEM RGB_GROUP_PAD_NUMLOCK[] = { 13, 14, 15, 30, 31, 32, 47, 48, 49};
const uint8_t PROGMEM RGB_GROUP_SIDE[] = { 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99 };

const uint8_t RGB_GROUP_FN_SIZE = 13;
const uint8_t RGB_GROUP_MOD_SIZE = 14;
const uint8_t RGB_GROUP_PAD_NUMLOCK_SIZE = 9;
const uint8_t RGB_GROUP_SIDE_SIZE = 10;

uint8_t os = 0;

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [MAC_BASE] = LAYOUT_tkl_ansi(
        KC_ESC,             KC_BRID,  KC_BRIU,  KC_MCTL,  KC_LPAD,   _______,  _______,  _______,  _______,  KC_MUTE,    KC_VOLD,  KC_VOLU,  BL_TOGG,     NUMX,  _______,  _______,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,         KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,    KC_MINS,   KC_EQL,  KC_BSPC,  _______,  _______,  _______,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,         KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,    KC_LBRC,  KC_RBRC,  KC_BSLS,  DM_PLY1,  DM_PLY2,  _______,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,         KC_G,     KC_H,     KC_J,     KC_K,     KC_L,  KC_SCLN,    KC_QUOT,   KC_ENT,
        KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,         KC_B,     KC_N,     KC_M,  KC_COMM,   KC_DOT,  KC_SLSH,    KC_RSFT,                                  KC_UP,
        KC_LCTL,  KC_LGUI,  KC_LALT,                        KC_SPC,                                          KC_RALT, MO(MAC_FN),  KC_RGUI,  KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [MAC_FN] = LAYOUT_tkl_ansi(
        QK_BOOT,              KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,    KC_F12,  RGB_MODX,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,   _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,   _______,  DM_REC1,  DM_REC2,  _______,
        KC_OSSW,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,                                  RGB_VAIX,
        _______,  _______,  _______,                      _______,                                         TG(GAME), TG(GAME),    _______,  _______, RGB_RMODX, RGB_VADX, RGB_MODX),

    [WIN_BASE] = LAYOUT_tkl_ansi(
        KC_ESC,             KC_BRID,  KC_BRIU,  KC_MCTL,  KC_LPAD,   _______,  _______,  _______,  _______,  KC_MUTE,    KC_VOLD,  KC_VOLU,  BL_TOGG,     NUMX,  _______,  _______,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,         KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,    KC_MINS,   KC_EQL,  KC_BSPC,  _______,  _______,  _______,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,         KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,    KC_LBRC,  KC_RBRC,  KC_BSLS,  DM_PLY1,  DM_PLY2,  _______,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,         KC_G,     KC_H,     KC_J,     KC_K,     KC_L,  KC_SCLN,    KC_QUOT,   KC_ENT,
        KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,         KC_B,     KC_N,     KC_M,  KC_COMM,   KC_DOT,  KC_SLSH,    KC_RSFT,                                  KC_UP,
        KC_LCTL,  KC_LGUI,  KC_LALT,                        KC_SPC,                                          KC_RALT, MO(WIN_FN),  KC_RGUI,  KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [WIN_FN] = LAYOUT_tkl_ansi(
        QK_BOOT,              KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,    KC_F12,  RGB_MODX,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,   _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,   _______,  DM_REC1,  DM_REC2,  _______,
        KC_OSSW,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,                                  RGB_VAIX,
        _______,  _______,  _______,                      _______,                                         TG(GAME), TG(GAME),    _______,  _______, RGB_RMODX, RGB_VADX, RGB_MODX),

    [GAME] = LAYOUT_tkl_ansi(
        _______,            _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,   _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,     KC_1,     KC_2,     KC_3,     KC_4,       KC_5,   _______,  _______,  _______,  _______,
        _______,  _______,     KC_O,  _______,  _______,  _______,  _______,     KC_R,     KC_E,     KC_W,  _______,   KC_TAB,    _______,   _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,     KC_F,     KC_A,     KC_S,     KC_D,  _______,    _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_LCTL,  KC_LSFT,                                      KC_C,
        _______,    KC_NO,  _______,                      _______,                                          _______,  _______,      KC_NO,   KC_LCTL,     KC_Z,     KC_X,     KC_V),

    [NP_FUN] = LAYOUT_tkl_ansi(
        _______,            _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,   _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,   _______,   KC_INS,  KC_HOME,  KC_PGUP,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,   _______,   KC_DEL,   KC_END,  KC_PGDN,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,                                   _______,
        _______,  _______,  _______,                      _______,                                          _______, _______,     _______,   _______,  _______,  _______,  _______)
};

// clang-format on
void housekeeping_task_user(void) {
    housekeeping_task_keychron();
}

void keyboard_post_init_user(void) {

}

void activate_num_keys(void) {
    /* No numpad on K8
    if (IS_LAYER_ON(NP_NUM)) {
        // Turn off numbers, turn on functions
        layer_off(NP_NUM);
        layer_on(NP_FUN);
    }
    else
    */
    if (IS_LAYER_ON(NP_FUN)) {
        // Turn off functions, back to base layer
        layer_off(NP_FUN);
    } else {
        // On base layer, turn on numbers
        layer_on(NP_FUN);
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    if (!record->event.pressed) {

        switch (keycode) {

            case NUMX:
                activate_num_keys();
                break;

            /*
             * These are overriding the default RGB keys to use the
             * no-eeprom functions.
             */
            case RGB_TOGX:
                led_matrix_toggle_noeeprom();
                break;
            case RGB_MODX:
                led_matrix_step_noeeprom();
                break;
            case RGB_RMODX:
                led_matrix_step_reverse_noeeprom();
                break;
            case RGB_VAIX:
                led_matrix_increase_val_noeeprom();
                break;
            case RGB_VADX:
                led_matrix_decrease_val_noeeprom();
                break;
            case RGB_SPIX:
                led_matrix_increase_speed_noeeprom();
                break;
            case RGB_SPDX:
                led_matrix_decrease_speed_noeeprom();
                break;
        }
    }
    return true;
}

// RGB hsv_to_rgb_keep_brightness(int h, int s, int v) {
//     HSV hsv = { h, s, v };
//     if (hsv.v > rgb_matrix_get_val()) {
//         hsv.v = rgb_matrix_get_val();
//     }
//     return hsv_to_rgb(hsv);
// }

bool led_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {

    uint32_t mode = led_matrix_get_mode();
    // assign colors if the matrix is on and the current mode
    // is SOLID COLORS => No animations running
    if(led_matrix_is_enabled() && mode == 1) { // && swap_colors) {

        for (uint8_t i = led_min; i < led_max; i++) {
            led_matrix_set_value(i, 255);
        }

        // FN group
        for (uint8_t i = 0; i < RGB_GROUP_FN_SIZE; i++) {
            led_matrix_set_value(RGB_GROUP_FN[i], 175);
        }
        // Mod group
        for (uint8_t i = 0; i < RGB_GROUP_MOD_SIZE; i++) {
            led_matrix_set_value(RGB_GROUP_MOD[i], 175);
        }
        // Pad group
        for (uint8_t i = 0; i < RGB_GROUP_PAD_NUMLOCK_SIZE; i++) {
            led_matrix_set_value(RGB_GROUP_PAD_NUMLOCK[i], 0);
        }

        if (IS_LAYER_ON(GAME)) {
            led_matrix_set_value(42, 100);
            led_matrix_set_value(58, 100);
            led_matrix_set_value(59, 100);
            led_matrix_set_value(60, 100);
        }

        /* No numpad on K8
        if (IS_LAYER_ON(NP_NUM)) {
            RGB numpad = hsv_to_rgb_keep_brightness(HSV_YELLOW);
            for (uint8_t i = 0; i < RGB_GROUP_PAD_NUMLOCK_SIZE; i++) {
                rgb_matrix_set_color(RGB_GROUP_PAD_NUMLOCK[i], numpad.r, numpad.g, numpad.b);
            }
        }
        else
        */
        if (IS_LAYER_ON(NP_FUN)) {
            for (uint8_t i = 0; i < RGB_GROUP_PAD_NUMLOCK_SIZE; i++) {
                led_matrix_set_value(RGB_GROUP_PAD_NUMLOCK[i], 255);
            }
        } else {

            /*
             * Macropad area
             */
            if (dynamic_macro_is_recording(1)) {
                led_matrix_set_value(47, 125);
            }
            else if (dynamic_macro_is_recorded(1)) {
                led_matrix_set_value(47, 255);
            }

            if (dynamic_macro_is_recording(-1)) {
                led_matrix_set_value(48, 125);
            }
            else if (dynamic_macro_is_recorded(-1)) {
                led_matrix_set_value(48, 255);
            }
        }

        // if (host_keyboard_led_state().caps_lock) {
        //     RGB game = hsv_to_rgb_keep_brightness(HSV_RED);
        //     rgb_matrix_set_color(14, game.r, game.g, game.b);
        // }
        // if (host_keyboard_led_state().num_lock) {
        //     RGB game = hsv_to_rgb_keep_brightness(HSV_RED);
        //     rgb_matrix_set_color(15, game.r, game.g, game.b);
        // }
        // if (host_keyboard_led_state().scroll_lock) {
        //     RGB game = hsv_to_rgb_keep_brightness(HSV_RED);
        //     rgb_matrix_set_color(16, game.r, game.g, game.b);
        // }

        if (detected_host_os() == OS_MACOS) {
            led_matrix_set_value(15, 255);
        } else {
            led_matrix_set_value(15, 125);
        }
    }
    return true;
}
