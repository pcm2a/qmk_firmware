/* Copyright 2021 Glorious, LLC <salman@pcgamingrace.com>
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
#include <os_detection.h>

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// enum custom_layers {
//     _BL,
//     _FL,
// };

enum my_keycodes {
  COL_KEYS = SAFE_RANGE,
  METEST,
  COL_MODS,
  FN_MAC,
  FN_WIN,
  NEO_MATRIX,
  RGB_TOGX,
  RGB_MODX,
  RGB_VAIX,
  RGB_HUIX,
  RGB_SAIX,
  RGB_SPIX,
  RGB_RMODX,
  RGB_VADX,
  RGB_HUDX,
  RGB_SADX,
  RGB_SPDX,

  NUMX
};

enum layers{
  MAC_BASE,
  MAC_FN,
  WIN_BASE,
  WIN_FN,
  GAME,
  NUMPAD_NUMS,
  NUMPAD_FUNCS
};

typedef union {
  uint32_t raw;
  struct {
    uint8_t keys_color;
    uint8_t mods_color;
    uint8_t pads_color;
    uint8_t side_color;
  };
} user_config_t;

user_config_t user_config;

const uint8_t PROGMEM RGB_GROUP_FN[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };
const uint8_t PROGMEM RGB_GROUP_MOD[] = { 18, 31, 36, 49, 54, 66, 70, 81, 87, 88, 89, 91, 92, 93 };
const uint8_t PROGMEM RGB_GROUP_PAD[] = { 14, 15, 16, 17, 32, 33, 34, 35, 50, 51, 52, 53, 67, 68, 69, 83, 84, 85, 86, 97, 98 };
const uint8_t PROGMEM RGB_GROUP_PAD_NUMLOCK[] = { 32, 33, 34, 35, 50, 51, 52, 53, 67, 68, 69, 83, 84, 85, 86, 97, 98 };
const uint8_t PROGMEM RGB_GROUP_SIDE[] = { 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120 };

const uint8_t RGB_GROUP_FN_SIZE = 14;
const uint8_t RGB_GROUP_MOD_SIZE = 14;
const uint8_t RGB_GROUP_PAD_SIZE = 21;
const uint8_t RGB_GROUP_PAD_NUMLOCK_SIZE = 17;
const uint8_t RGB_GROUP_SIDE_SIZE = 20;

const uint8_t COLORS_SIZE = 12;
const uint8_t PROGMEM COLORS[][3] = { {HSV_BLUE}, {HSV_WHITE}, {HSV_YELLOW}, {HSV_RED}, {HSV_GREEN}, {HSV_ORANGE}, {HSV_AZURE}, {HSV_CORAL}, {HSV_CYAN}, {HSV_GOLDENROD}, {HSV_MAGENTA}, {HSV_PURPLE} };
const uint8_t DEFAULT_KEYS_COLOR = 1;
const uint8_t DEFAULT_MODS_COLOR = 2;
const uint8_t DEFAULT_PADS_COLOR = 4;
const uint8_t DEFAULT_SIDE_COLOR = 0;

uint8_t keys_color[3];
uint8_t mods_color[3];
uint8_t pads_color[3];
uint8_t side_color[3];

uint8_t os = 0;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    // QK_GESC,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_PSCR,  KC_DEL,   KC_INS,   KC_PGUP,  KC_PGDN,

    /* Keymap _BL: Base Layer (Default Layer) */
    [MAC_BASE] = LAYOUT(
        KC_ESC,   KC_BRID,  KC_BRIU,  KC_MCTL,  KC_LPAD,  COL_KEYS, COL_MODS,  _______,  _______,  KC_MUTE,    KC_VOLD, KC_VOLU,  RGB_TOG, RGB_MODX,   KC_DEL,  KC_INS,  KC_PGUP,  KC_PGDN,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,         KC_5,     KC_6,     KC_7,     KC_8,     KC_9,       KC_0, KC_MINS,   KC_EQL,  KC_BSPC,     NUMX, KC_PSLS,  KC_PAST,  KC_PMNS,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,         KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,       KC_P, KC_LBRC,  KC_RBRC,  KC_BSLS,    KC_P7,   KC_P8,    KC_P9,  KC_PPLS,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,         KC_G,     KC_H,     KC_J,     KC_K,     KC_L,    KC_SCLN, KC_QUOT,             KC_ENT,    KC_P4,   KC_P5,    KC_P6,
        KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,         KC_B,     KC_N,     KC_M,  KC_COMM,   KC_DOT,    KC_SLSH, KC_RSFT,              KC_UP,  DM_PLY1,  DM_PLY2,   KC_P3,  KC_PENT,
        KC_LCTL,  KC_LGUI,  KC_LALT,                        KC_SPC,                                KC_RALT, MO(MAC_FN), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT,   KC_P0,  KC_PDOT),

    [MAC_FN] = LAYOUT(
        QK_BOOT,    KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,    KC_F12,   _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,   _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,   _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,             _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,              RGB_VAIX,   DM_REC1,  DM_REC2, DM_RSTP,  _______,
        _______,  _______,   _______,                      _______,                               TG(GAME), _______,  TG(GAME), RGB_RMODX,  RGB_VADX, RGB_MODX,  _______,  _______),

    [WIN_BASE] = LAYOUT(
        KC_ESC,   KC_BRID,  KC_BRIU,  KC_MCTL,  KC_LPAD,  COL_KEYS, COL_MODS,  _______,  _______,  KC_MUTE,    KC_VOLD, KC_VOLU,  RGB_TOG, RGB_MODX,   KC_DEL,  KC_INS,  KC_PGUP,  KC_PGDN,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,         KC_5,     KC_6,     KC_7,     KC_8,     KC_9,       KC_0, KC_MINS,   KC_EQL,  KC_BSPC,     NUMX, KC_PSLS,  KC_PAST,  KC_PMNS,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,         KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,       KC_P, KC_LBRC,  KC_RBRC,  KC_BSLS,    KC_P7,   KC_P8,    KC_P9,  KC_PPLS,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,         KC_G,     KC_H,     KC_J,     KC_K,     KC_L,    KC_SCLN, KC_QUOT,             KC_ENT,    KC_P4,   KC_P5,    KC_P6,
        KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,         KC_B,     KC_N,     KC_M,  KC_COMM,   KC_DOT,    KC_SLSH, KC_RSFT,              KC_UP,  DM_PLY1,  DM_PLY2,   KC_P3,  KC_PENT,
        KC_LCTL,  KC_LGUI,  KC_LALT,                        KC_SPC,                                KC_RALT, MO(MAC_FN), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT,   KC_P0,  KC_PDOT),

    [WIN_FN] = LAYOUT(
        QK_BOOT,    KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,    KC_F12,   _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,   _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,   _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,             _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,              RGB_VAIX,  DM_REC1,  DM_REC2,  DM_RSTP,  _______,
        _______,  _______,   _______,                      _______,                                TG(GAME), _______,  TG(GAME), RGB_RMODX, RGB_VADX, RGB_MODX,  _______,  _______),

    [GAME] = LAYOUT(
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,     KC_1,     KC_2,     KC_3,     KC_4,     KC_5,   _______,  _______,  _______,  _______,  _______,
        _______,  _______,     KC_O,  _______,  _______,  _______,  _______,     KC_R,     KC_E,     KC_W,  _______,   KC_TAB,  _______,   _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,     KC_F,     KC_A,     KC_S,     KC_D,  _______,  _______,             _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_LCTL,  KC_LSFT,                KC_C,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,                      _______,                                _______,  _______,  KC_LCTL,     KC_Z,      KC_X,     KC_V,  _______,  _______),

    [NUMPAD_NUMS] = LAYOUT(
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_SLSH,  KC_ASTR,  KC_MINS,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,     KC_7,     KC_8,     KC_9,  KC_PLUS,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,               KC_4,     KC_5,     KC_6,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,               KC_1,     KC_2,     KC_3,   KC_ENT,
        _______,  _______,  _______,                      _______,                                _______,  _______,  _______,  _______,  _______,  _______,     KC_0,   KC_DOT),

    [NUMPAD_FUNCS] = LAYOUT(
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_SLSH,  KC_ASTR,  KC_MINS,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_HOME,    KC_UP,  KC_PGUP,  KC_PLUS,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            KC_LEFT,    KC_NO,  KC_RGHT,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,             KC_END,  KC_DOWN,  KC_PGDN,   KC_ENT,
        _______,  _______,  _______,                      _______,                                _______,  _______,  _______,  _______,  _______,  _______,   KC_INS,   KC_DEL)
};


void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  //debug_enable=true;
  //debug_matrix=true;
  //debug_keyboard=true;
  //debug_mouse=true;

  // Should load from eeprom here...
  user_config.raw = 0;
  user_config.keys_color = DEFAULT_KEYS_COLOR;
  user_config.mods_color = DEFAULT_MODS_COLOR;
  user_config.pads_color = DEFAULT_PADS_COLOR;
  user_config.side_color = DEFAULT_SIDE_COLOR;

  keys_color[0] = pgm_read_byte(&COLORS[user_config.keys_color][0]);
  keys_color[1] = pgm_read_byte(&COLORS[user_config.keys_color][1]);
  keys_color[2] = pgm_read_byte(&COLORS[user_config.keys_color][2]);

  mods_color[0] = pgm_read_byte(&COLORS[user_config.mods_color][0]);
  mods_color[1] = pgm_read_byte(&COLORS[user_config.mods_color][1]);
  mods_color[2] = pgm_read_byte(&COLORS[user_config.mods_color][2]);

  pads_color[0] = pgm_read_byte(&COLORS[user_config.pads_color][0]);
  pads_color[1] = pgm_read_byte(&COLORS[user_config.pads_color][1]);
  pads_color[2] = pgm_read_byte(&COLORS[user_config.pads_color][2]);

  side_color[0] = pgm_read_byte(&COLORS[user_config.side_color][0]);
  side_color[1] = pgm_read_byte(&COLORS[user_config.side_color][1]);
  side_color[2] = pgm_read_byte(&COLORS[user_config.side_color][2]);
}

void do_the_thing(void) {
    tap_code(KC_NUM);
    tap_code(KC_NUM);
}

void cycle_keys_color(void) {

  uint32_t mode = rgblight_get_mode();
  // make sure the backlight is on
  if(rgb_matrix_config.enable == 1) {
    // If the mode is already solid, then switch the key color
    if (mode == 1) {

      if (user_config.keys_color + 1 == COLORS_SIZE) {
        user_config.keys_color = 0;
      } else {
        user_config.keys_color++;
      }
      keys_color[0] = pgm_read_byte(&COLORS[user_config.keys_color][0]);
      keys_color[1] = pgm_read_byte(&COLORS[user_config.keys_color][1]);
      keys_color[2] = pgm_read_byte(&COLORS[user_config.keys_color][2]);

    } else {
      rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
    }
  }
}

void cycle_mods_color(void) {

  uint32_t mode = rgblight_get_mode();
  // make sure the backlight is on
  if(rgb_matrix_config.enable == 1) {
    // If the mode is already solid, then switch the key color
    if (mode == 1) {

      if (user_config.mods_color + 1 == COLORS_SIZE) {
        user_config.mods_color = 0;
      } else {
        user_config.mods_color++;
      }
      mods_color[0] = pgm_read_byte(&COLORS[user_config.mods_color][0]);
      mods_color[1] = pgm_read_byte(&COLORS[user_config.mods_color][1]);
      mods_color[2] = pgm_read_byte(&COLORS[user_config.mods_color][2]);

    } else {
      rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
    }
  }
}

void activate_num_keys(void) {
    if (IS_LAYER_ON(NUMPAD_NUMS)) {
        // Turn off numbers, turn on functions
        layer_off(NUMPAD_NUMS);
        layer_on(NUMPAD_FUNCS);
    }
    else if (IS_LAYER_ON(NUMPAD_FUNCS)) {
        // Turn off functions, back to base layer
        layer_off(NUMPAD_FUNCS);
    } else {
        // On base layer, turn on numbers
        layer_on(NUMPAD_NUMS);
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) {
        switch (keycode) {
          case METEST:
                do_the_thing();
                break;

            case COL_KEYS:
                cycle_keys_color();
                break;
            case COL_MODS:
                cycle_mods_color();
                break;

            case NUMX:
                activate_num_keys();
                break;

            /*
             * These are overriding the default RGB keys to use the
             * no-eeprom functions.
             */
            case RGB_TOGX:
                rgb_matrix_toggle_noeeprom();
                break;
            case RGB_MODX:
                rgb_matrix_step_noeeprom();
                break;
            case RGB_RMODX:
                rgb_matrix_step_reverse_noeeprom();
                break;
            case RGB_HUIX:
                rgb_matrix_increase_hue_noeeprom();
                break;
            case RGB_HUDX:
                rgb_matrix_decrease_hue_noeeprom();
                break;
            case RGB_SAIX:
                rgb_matrix_increase_sat_noeeprom();
                break;
            case RGB_SADX:
                rgb_matrix_decrease_sat_noeeprom();
                break;
            case RGB_VAIX:
                rgb_matrix_increase_val_noeeprom();
                break;
            case RGB_VADX:
                rgb_matrix_decrease_val_noeeprom();
                break;
            case RGB_SPIX:
                rgb_matrix_increase_speed_noeeprom();
                break;
            case RGB_SPDX:
                rgb_matrix_decrease_speed_noeeprom();
                break;
        }
    }
    return true;
}

void dynamic_macro_record_start_user(int8_t direction) {

}

void dynamic_macro_play_user(int8_t direction) {

}

void dynamic_macro_record_end_user(int8_t direction) {

}

RGB hsv_to_rgb_keep_brightness(int h, int s, int v) {
    HSV hsv = { h, s, v };
    if (hsv.v > rgb_matrix_get_val()) {
        hsv.v = rgb_matrix_get_val();
    }
    return hsv_to_rgb(hsv);
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {

    uint32_t mode = rgblight_get_mode();
    // assign colors if the matrix is on and the current mode
    // is SOLID COLORS => No animations running
    if(rgb_matrix_config.enable == 1 && mode == 1) { // && swap_colors) {

        RGB rgb_color = hsv_to_rgb_keep_brightness(keys_color[0], keys_color[1], keys_color[2]);
        RGB rgb_mods_color = hsv_to_rgb_keep_brightness(mods_color[0], mods_color[1], mods_color[2]);
        //RGB rgb_pads_color = hsv_to_rgb_keep_brightness(pads_color[0], pads_color[1], pads_color[2]);
        RGB rgb_side_color = hsv_to_rgb_keep_brightness(side_color[0], side_color[1], side_color[2]);

        for (uint8_t i = led_min; i < led_max; i++) {
            rgb_matrix_set_color(i, rgb_color.r, rgb_color.g, rgb_color.b);
        }

        // FN group
        for (uint8_t i = 0; i < RGB_GROUP_FN_SIZE; i++) {
            rgb_matrix_set_color(RGB_GROUP_FN[i], rgb_mods_color.r, rgb_mods_color.g, rgb_mods_color.b);
        }
        // Mod group
        for (uint8_t i = 0; i < RGB_GROUP_MOD_SIZE; i++) {
            rgb_matrix_set_color(RGB_GROUP_MOD[i], rgb_mods_color.r, rgb_mods_color.g, rgb_mods_color.b);
        }
        // Pad group
        for (uint8_t i = 0; i < RGB_GROUP_PAD_SIZE; i++) {
            //rgb_matrix_set_color(RGB_GROUP_PAD[i], rgb_pads_color.r, rgb_pads_color.g, rgb_pads_color.b);
            rgb_matrix_set_color(RGB_GROUP_PAD[i], 0, 0, 0);
        }
        // Side lights
        for (uint8_t i = 0; i < RGB_GROUP_SIDE_SIZE; i++) {
            rgb_matrix_set_color(RGB_GROUP_SIDE[i], rgb_side_color.r, rgb_side_color.g, rgb_side_color.b);
        }

        if (IS_LAYER_ON(GAME)) {
            RGB game = hsv_to_rgb_keep_brightness(HSV_RED);
            rgb_matrix_set_color(45, game.r, game.g, game.b);
            rgb_matrix_set_color(62, game.r, game.g, game.b);
            rgb_matrix_set_color(63, game.r, game.g, game.b);
            rgb_matrix_set_color(64, game.r, game.g, game.b);
        }

        if (IS_LAYER_ON(NUMPAD_NUMS)) {
            RGB numpad = hsv_to_rgb_keep_brightness(HSV_YELLOW);
            for (uint8_t i = 0; i < RGB_GROUP_PAD_NUMLOCK_SIZE; i++) {
                rgb_matrix_set_color(RGB_GROUP_PAD_NUMLOCK[i], numpad.r, numpad.g, numpad.b);
            }
        }
        else if (IS_LAYER_ON(NUMPAD_FUNCS)) {
            RGB numpad = hsv_to_rgb_keep_brightness(HSV_RED);
            for (uint8_t i = 0; i < RGB_GROUP_PAD_NUMLOCK_SIZE; i++) {
                rgb_matrix_set_color(RGB_GROUP_PAD_NUMLOCK[i], numpad.r, numpad.g, numpad.b);
            }
        }

        if (host_keyboard_led_state().caps_lock) {
            RGB game = hsv_to_rgb_keep_brightness(HSV_RED);
            rgb_matrix_set_color(14, game.r, game.g, game.b);
        }
        if (host_keyboard_led_state().num_lock) {
            RGB game = hsv_to_rgb_keep_brightness(HSV_RED);
            rgb_matrix_set_color(15, game.r, game.g, game.b);
        }
        if (host_keyboard_led_state().scroll_lock) {
            RGB game = hsv_to_rgb_keep_brightness(HSV_RED);
            rgb_matrix_set_color(16, game.r, game.g, game.b);
        }

        if (detected_host_os() == OS_MACOS) {
            RGB color = hsv_to_rgb_keep_brightness(HSV_YELLOW);
            rgb_matrix_set_color(17, color.r, color.g, color.b);
        } else {
            RGB color = hsv_to_rgb_keep_brightness(HSV_BLUE);
            rgb_matrix_set_color(17, color.r, color.g, color.b);
        }
    }
    return true;
}
