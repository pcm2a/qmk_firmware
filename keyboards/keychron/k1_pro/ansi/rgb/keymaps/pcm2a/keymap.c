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
#include <print.h>
#include <os_detection.h>

enum layers{
  MAC_BASE,
  MAC_FN,
  WIN_BASE,
  WIN_FN,
  GAME
};

enum my_keycodes {
  COL_KEYS = NEW_SAFE_RANGE,
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
  RGB_SPDX
};

typedef union {
  uint32_t raw;
  struct {
    uint8_t keys_color;
    uint8_t mods_color;
  };
} user_config_t;

user_config_t user_config;

const uint8_t COLORS_SIZE = 12;
const uint8_t PROGMEM COLORS[][3] = { {HSV_BLUE}, {HSV_WHITE}, {HSV_YELLOW}, {HSV_RED}, {HSV_GREEN}, {HSV_ORANGE}, {HSV_AZURE}, {HSV_CORAL}, {HSV_CYAN}, {HSV_GOLDENROD}, {HSV_MAGENTA}, {HSV_PURPLE} };
const uint8_t DEFAULT_KEYS_COLOR = 1;
const uint8_t DEFAULT_MODS_COLOR = 3;

uint8_t keys_color[3];
uint8_t mods_color[3];

uint16_t fn_timer = 0;

/*
const uint8_t PROGMEM RGB_GROUP_ESC[] = { 0 };
const uint8_t PROGMEM RGB_GROUP_FN[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
const uint8_t PROGMEM RGB_GROUP_MOD[] = { 16, 29, 33, 46, 50, 62, 63, 74, 76, 77, 78, 80, 81, 82, 83 };
const uint8_t PROGMEM RGB_GROUP_ABC[] = { 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 79 };
const uint8_t PROGMEM RGB_GROUP_OTHER[] = { 13, 14, 15, 30, 31, 32, 47, 48, 49, 75, 84, 85, 86 };
 */
const uint8_t PROGMEM RGB_GROUP_FN[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };

uint8_t eye_length = 5;
uint8_t eye_color[3] = {HSV_RED};
uint16_t eye_speed = 200;
uint8_t eye_position = 0;
uint8_t eye_keycode_size = 16;
uint8_t eye_direction = 0; // 0 = right ->, 1 = left <-
uint16_t eye_timer = 0;
const uint8_t PROGMEM EYE_KEYCODES[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

uint8_t snake_length = 6;
uint8_t snake_color[3] = {HSV_BLUE};
uint8_t snake_speed = 100;
uint8_t snake_position = 5;
uint8_t snake_keycode_size = 87;
uint16_t snake_timer = 0;
const uint8_t PROGMEM SNAKE_KEYCODES[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16,
    33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
    62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50,
    63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75,
    86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76 };

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [MAC_BASE] = LAYOUT_tkl_ansi(
        KC_ESC,             KC_BRID,  KC_BRIU,  KC_MCTL,  KC_LPAD,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  KC_SNAP,  KC_SIRI,  RGB_MODX,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,  KC_INS,   KC_HOME,  KC_PGUP,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_DEL,   KC_END,   KC_PGDN,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,            KC_UP,
        KC_LCTL,  KC_LOPTN, KC_LCMMD,                               KC_SPC,                                 KC_RCMMD, KC_ROPTN, FN_MAC,   KC_RCTL, KC_LEFT,  KC_DOWN,  KC_RGHT),

    [MAC_FN] = LAYOUT_tkl_ansi(
        _______,             KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   _______,  _______,  RGB_TOGX,
        COL_KEYS, BT_HST1,   BT_HST2,  BT_HST3,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
        RGB_TOGX, RGB_MODX,  RGB_VAIX, RGB_HUIX, RGB_SAIX, RGB_SPIX, _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  RGB_RMODX, RGB_VADX, RGB_HUDX, RGB_SADX, RGB_SPDX, _______,  _______,  _______,  _______,  _______,  _______,            _______,
        _______,             COL_MODS, _______,  _______,  _______,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,            _______,            _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  TG(GAME),  _______,  _______,  NEO_MATRIX),

    [WIN_BASE] = LAYOUT_tkl_ansi(
        KC_ESC,             KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_PSCR,  KC_CTANA, RGB_MODX,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,  KC_INS,   KC_HOME,  KC_PGUP,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_DEL,   KC_END,   KC_PGDN,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,            KC_UP,
        KC_LCTL,  KC_LWIN,  KC_LALT,                                KC_SPC,                                 KC_RALT,  KC_RGUI, FN_WIN,    KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [WIN_FN] = LAYOUT_tkl_ansi(
        _______,             KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  _______,  _______,  RGB_TOGX,
        _______,  BT_HST1,   BT_HST2,  BT_HST3,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
        RGB_TOGX,  RGB_MODX, RGB_VAIX, RGB_HUIX, RGB_SAIX, RGB_SPIX, _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  RGB_RMODX, RGB_VADX, RGB_HUDX, RGB_SADX, RGB_SPDX, _______,  _______,  _______,  _______,  _______,  _______,            _______,
        _______,             COL_MODS, _______,  _______,  _______,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,            _______,            _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  _______,  _______),

    [GAME] = LAYOUT_tkl_ansi(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, TO(0),
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_O, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_W, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS)
};

rgb_config_t rgb_matrix_config;

void matrix_init_user(void) {
  rgb_matrix_config.raw = pgm_read_dword(EECONFIG_RGB_MATRIX);
}

RGB hsv_to_rgb_keep_brightness(int h, int s, int v) {
    HSV hsv = { h, s, v };
    if (hsv.v > rgb_matrix_get_val()) {
        hsv.v = rgb_matrix_get_val();
    }
    return hsv_to_rgb(hsv);
}

void do_the_thing(void) {

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

void do_the_thing2(void) {

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

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) {
        switch (keycode) {
          case COL_KEYS:
                do_the_thing();
                break;
            case COL_MODS:
                do_the_thing2();
                break;
            case NEO_MATRIX:
                rgb_matrix_mode(RGB_MATRIX_DIGITAL_RAIN_RELOADED);
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

    switch (keycode) {
        case FN_MAC:
            if (record->event.pressed) {
                layer_on(MAC_FN);
            } else {
                layer_off(MAC_FN);
            }
            break;
            //return false; // Don't let the keypress be handled further
        case FN_WIN:
          if (record->event.pressed) {
            layer_on(WIN_FN);
          } else {
            layer_off(WIN_FN);
          }
          break;
          //return false;
    }
    return true;
}

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=true;
  debug_matrix=true;
  //debug_keyboard=true;
  //debug_mouse=true;

  // Should load from eeprom here...
  user_config.raw = 0;
  user_config.keys_color = DEFAULT_KEYS_COLOR;
  user_config.mods_color = DEFAULT_MODS_COLOR;

  keys_color[0] = pgm_read_byte(&COLORS[user_config.keys_color][0]);
  keys_color[1] = pgm_read_byte(&COLORS[user_config.keys_color][1]);
  keys_color[2] = pgm_read_byte(&COLORS[user_config.keys_color][2]);

  mods_color[0] = pgm_read_byte(&COLORS[user_config.mods_color][0]);
  mods_color[1] = pgm_read_byte(&COLORS[user_config.mods_color][1]);
  mods_color[2] = pgm_read_byte(&COLORS[user_config.mods_color][2]);
}

void cylon(void) {

    if (eye_timer == 0) {
        eye_timer = timer_read();
    }
    else if (timer_elapsed(eye_timer) > eye_speed) {
        // Move the eye
        if (eye_direction == 0) {
            // Moving to the right
            if (eye_position + eye_length == eye_keycode_size) {
                // Reached the right end
                eye_direction = 1;
            } else {
                eye_position++;
            }
        } else {
            // Moving to the left
            if (eye_position - 1 < 0) {
                // Reached the left start
                eye_direction = 0;
            } else {
                eye_position--;
            }
        }

        eye_timer = timer_read();
    }

    // Draw the eye
    RGB rgb = hsv_to_rgb_keep_brightness(eye_color[0], eye_color[1], eye_color[2]);

    for (uint8_t i = 0; i < eye_keycode_size; i++) {
        if (i >= eye_position && i < eye_position + eye_length) {
            rgb_matrix_set_color(pgm_read_byte(&EYE_KEYCODES[i]), rgb.r, rgb.g, rgb.b);
        } else {
            rgb_matrix_set_color(pgm_read_byte(&EYE_KEYCODES[i]), 0, 0, 0);
        }
    }
}

void snake(void) {

    if (snake_timer == 0) {
        snake_timer = timer_read();
    }
    else if (timer_elapsed(snake_timer) > snake_speed) {
        if (snake_position + 1 == snake_keycode_size) {
            snake_position = 0;
        } else {
            snake_position++;
        }
        snake_timer = timer_read();
    }

    // Draw the eye
    RGB rgb = hsv_to_rgb_keep_brightness(snake_color[0], snake_color[1], snake_color[2]);

    for (uint8_t i = snake_position; i < snake_position + snake_length; i++) {
        if (i < snake_keycode_size) {
            rgb_matrix_set_color(pgm_read_byte(&SNAKE_KEYCODES[i]), rgb.r, rgb.g, rgb.b);
        } else {
            rgb_matrix_set_color(pgm_read_byte(&SNAKE_KEYCODES[ i - snake_keycode_size ]), rgb.r, rgb.g, rgb.b);
        }
    }
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {

    uint32_t mode = rgblight_get_mode();
    // assign colors if the matrix is on and the current mode
    // is SOLID COLORS => No animations running
    if(rgb_matrix_config.enable == 1 && mode == 1) { // && swap_colors) {

        uint8_t layer = biton32(layer_state);
        if (layer == 0) {
            //hsv = {130, 255, 255};
        } else {
            //hsv = {30, 255, 255};
        }

        RGB rgb_color = hsv_to_rgb_keep_brightness(keys_color[0], keys_color[1], keys_color[2]);
        RGB rgb_mods_color = hsv_to_rgb_keep_brightness(mods_color[0], mods_color[1], mods_color[2]);

        for (uint8_t i = led_min; i < led_max; i++) {
            if (HAS_FLAGS(g_led_config.flags[i], 0x01) || i == 50) { // 0x01 == LED_FLAG_MODIFIER
                rgb_matrix_set_color(i, rgb_mods_color.r, rgb_mods_color.g, rgb_mods_color.b);
            } else {
                rgb_matrix_set_color(i, rgb_color.r, rgb_color.g, rgb_color.b);
            }
        }

        if (IS_LAYER_ON(GAME)) {
            RGB game = hsv_to_rgb_keep_brightness(HSV_RED);
            rgb_matrix_set_color(42, game.r, game.g, game.b);
            rgb_matrix_set_color(58, game.r, game.g, game.b);
            rgb_matrix_set_color(59, game.r, game.g, game.b);
            rgb_matrix_set_color(60, game.r, game.g, game.b);

            cylon();
        }

        // if (IS_LAYER_ON(MAC_BASE)) {
        //     snake();
        // }

        if (IS_LAYER_ON(MAC_FN) || IS_LAYER_ON(WIN_FN)) {

            //uprintf("CAM %d\n", detected_host_os());

            RGB fn = hsv_to_rgb_keep_brightness(HSV_GREEN);

            if (get_highest_layer(layer_state) > 0) {
                uint8_t layer = get_highest_layer(layer_state);

                for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
                    for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
                        uint8_t index = g_led_config.matrix_co[row][col];
                        if (index >= led_min && index < led_max && index != NO_LED &&
                        keymap_key_to_keycode(layer, (keypos_t){col,row}) > KC_TRNS) {
                            rgb_matrix_set_color(index, fn.r, fn.g, fn.b);
                        }
                    }
                }
            }

            if (fn_timer == 0) {
                fn_timer = timer_read();
            }
            if (timer_elapsed(fn_timer) < 500) {
                rgb_matrix_set_color(pgm_read_byte(&RGB_GROUP_FN[user_config.keys_color]), 0, 0, 0);
                rgb_matrix_set_color(pgm_read_byte(&RGB_GROUP_FN[user_config.mods_color]), 0, 0, 0);
            } else if (timer_elapsed(fn_timer) > 1000) {
                fn_timer = 0;
            } else {
                rgb_matrix_set_color(pgm_read_byte(&RGB_GROUP_FN[user_config.keys_color]), rgb_color.r, rgb_color.g, rgb_color.b);
                rgb_matrix_set_color(pgm_read_byte(&RGB_GROUP_FN[user_config.mods_color]), rgb_mods_color.r, rgb_mods_color.g, rgb_mods_color.b);
            }
        }
    }
	return true;
}
