/* Copyright 2020 unrelenting.technology
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
#include "timer.h"

// Define timeout values
#define BACKLIGHT_TIMEOUT_MS 120000 //300000 // 5 minutes
#define BACKLIGHT_CHECK_INTERVAL_MS 30000 //30000 // 30 seconds

#define LED_PIN D1

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE,
    _FN,
    _GAME
};

enum custom_keycodes {
    BL_TOGX = SAFE_RANGE,
    BL_DOWX,
    BL_UPX
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        QK_GESC,          KC_BRID, KC_BRIU, KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_MUTE, KC_VOLD, KC_VOLU, KC_F12,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_QUOT, KC_SCLN,          KC_ENT,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_RSFT, KC_UP,   KC_SLSH,
        KC_LCTL, KC_LGUI, KC_LALT, MO(_FN),          KC_SPC,           KC_SPC,           KC_RALT, KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
    ),
    [_FN] = LAYOUT(
        QK_BOOT,          KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, BL_DOWX, BL_UPX,   KC_DEL,
        KC_NUM,  XXXXXXX, KC_UP,   XXXXXXX, XXXXXXX, XXXXXXX, KC_P7,   KC_P8,   KC_P9,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        BL_TOGX,  KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX, XXXXXXX, KC_P4,   KC_P5,   KC_P6,   XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,
        KC_SCRL,          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_P1,   KC_P2,   KC_P3,   XXXXXXX, KC_SLSH, KC_PGUP, XXXXXXX,
        XXXXXXX, XXXXXXX, TG(_GAME), XXXXXXX,          XXXXXXX,          KC_P0,          KC_PDOT, KC_PENT, KC_HOME, KC_PGDN, KC_END
    ),
    [_GAME] = LAYOUT(
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    _______,
        _______, _______, QK_MOUSE_CURSOR_UP,    _______, _______, _______, _______, KC_R,    KC_E,    KC_W,    KC_Q,    KC_TAB,  KC_M,    _______,
        _______, QK_MOUSE_CURSOR_LEFT, QK_MOUSE_CURSOR_DOWN, QK_MOUSE_CURSOR_RIGHT, _______, _______, KC_G,    KC_F,    KC_A,    KC_S,    _______, KC_D,             _______,
        _______,          _______, _______, _______, KC_M,    _______, KC_V,    KC_C,    KC_X,    KC_Z,    KC_LSFT, KC_LCTL, _______,
        _______, _______, _______, _______,          _______,          _______,          _______, _______, KC_LCTL, _______, _______
    )
};

// Variables to track backlight state and timer
bool backlight_on = true; // Track if backlight is manually on or off
bool auto_backlight_disabled = false; // Track if the timeout disabled the backlight
uint32_t last_activity_timer = 0; // Track last activity time
uint32_t check_interval_timer = 0; // Track the last check interval time

void keyboard_pre_init_user(void) {
    setPinOutput(LED_PIN);
    writePinLow(LED_PIN);  // Start with the LED off
}

layer_state_t layer_state_set_user(layer_state_t state) {
    if (get_highest_layer(state) == 2) {
        writePinHigh(LED_PIN);
    } else {
        writePinLow(LED_PIN);
    }
    return state;
}

// Initialize backlight timeout
void backlight_init_user(void) {
    last_activity_timer = 1;
    check_interval_timer = timer_read();
}

// Check for key press activity to reset the timer
void matrix_scan_user(void) {

    if (!auto_backlight_disabled && timer_elapsed(check_interval_timer) >= BACKLIGHT_CHECK_INTERVAL_MS) {
        uprintf("checking every 30 seconds : %d\n", timer_elapsed(last_activity_timer));
        check_interval_timer = timer_read(); // Reset only the check interval timer

        last_activity_timer += BACKLIGHT_CHECK_INTERVAL_MS;

        // If no keypress in BACKLIGHT_TIMEOUT_MS, turn off backlight automatically
        if (last_activity_timer >= BACKLIGHT_TIMEOUT_MS) {
            print("disabling the light\n");
            //backlight_set(0); // Turn off backlight
            backlight_disable_noeeprom();
            auto_backlight_disabled = true;
        }
    }
}

// Override backlight toggle function to track manual on/off
void light_toggle(void) {
    backlight_on = !backlight_on; // Toggle manual backlight state
    if (backlight_on) {
        print("backlight on\n");
        // backlight_set(BACKLIGHT_LEVEL);
        backlight_enable_noeeprom();
    } else {
        print("backlight off\n");
        // backlight_set(0);
        backlight_disable_noeeprom();
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  if (record->event.pressed) {

    switch (keycode) {
        case BL_TOGX:
            // backlight_toggle_noeeprom();
            light_toggle();
            break;
        case BL_DOWX:
            backlight_decrease_noeeprom();
            break;
        case BL_UPX:
            backlight_increase_noeeprom();
            break;
    }

    // If the backlight is off due to timeout and was manually on before, turn it on
    if (auto_backlight_disabled && backlight_on) {
        print("key press - turning on\n");
        // backlight_set(BACKLIGHT_LEVEL);
        backlight_enable_noeeprom();
        auto_backlight_disabled = false;
    }
    // Reset the inactivity timer
    last_activity_timer = 1;
    uprintf("timer is %d\n", last_activity_timer);
  }
  return true;
}
