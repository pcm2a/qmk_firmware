/* Copyright 2020 Adam Honse <calcprogrammer1@gmail.com>
 * Copyright 2020 Dimitris Mantzouranis <d3xter93@gmail.com>
 * Copyright 2022 Harrison Chan (Xelus)
 * Copyright 2022 Thomas Bowman Mørch (tbowmo)
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
  NP_NUM,
  NP_FUN,
  NP_DEL,
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

const uint8_t PROGMEM RGB_GROUP_FN[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
const uint8_t PROGMEM RGB_GROUP_MOD[] = { 16, 33, 46, 50, 62, 63, 74, 76, 77, 78, 80, 81, 82, 83};
const uint8_t PROGMEM RGB_GROUP_PAD_NUMLOCK[] = { 13, 14, 15, 30, 31, 32, 47, 48, 49};

const uint8_t RGB_GROUP_FN_SIZE = 13;
const uint8_t RGB_GROUP_MOD_SIZE = 14;
const uint8_t RGB_GROUP_PAD_NUMLOCK_SIZE = 9;

const uint8_t COLORS_SIZE = 12;
const uint8_t PROGMEM COLORS[][3] = { {HSV_BLUE}, {HSV_WHITE}, {HSV_YELLOW}, {HSV_RED}, {HSV_GREEN}, {HSV_ORANGE}, {HSV_AZURE}, {HSV_CORAL}, {HSV_CYAN}, {HSV_GOLDENROD}, {HSV_MAGENTA}, {HSV_PURPLE} };
const uint8_t DEFAULT_KEYS_COLOR = 1;
const uint8_t DEFAULT_MODS_COLOR = 2;
const uint8_t DEFAULT_PADS_COLOR = 3;

uint8_t keys_color[3];
uint8_t mods_color[3];
uint8_t pads_color[3];

uint8_t os = 0;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [MAC_BASE] = LAYOUT_tkl_ansi(
        KC_ESC,             KC_BRID,  KC_BRIU,  KC_MCTL,  KC_LPAD,  COL_KEYS, COL_MODS,  _______,  _______,  KC_MUTE,   KC_VOLD, KC_VOLU,   RGB_TOG,      NUMX,  _______,  _______,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,         KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,   KC_MINS,   KC_EQL,  KC_BSPC,   _______,  _______,  _______,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,         KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,   KC_LBRC,  KC_RBRC,  KC_BSLS,   DM_PLY1,  DM_PLY2,  _______,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,         KC_G,     KC_H,     KC_J,     KC_K,     KC_L,  KC_SCLN,   KC_QUOT,   KC_ENT,
        KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,         KC_B,     KC_N,     KC_M,  KC_COMM,   KC_DOT,  KC_SLSH,   KC_RSFT,                                   KC_UP,
        KC_LCTL,  KC_LGUI,  KC_LALT,                        KC_SPC,                                          KC_RALT,   KC_RGUI, MO(MAC_FN), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [MAC_FN] = LAYOUT_tkl_ansi(
        QK_BOOT,              KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,    KC_F12,  RGB_MODX,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,   _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,   _______,  DM_REC1,  DM_REC2,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,                                  RGB_VAIX,
        _______,  _______,  _______,                      _______,                                         TG(GAME), TG(GAME),    _______,  _______, RGB_RMODX, RGB_VADX, RGB_MODX),

    [WIN_BASE] = LAYOUT_tkl_ansi(
        KC_ESC,             KC_BRID,  KC_BRIU,  KC_MCTL,  KC_LPAD,  COL_KEYS, COL_MODS,  _______,  _______,  KC_MUTE,   KC_VOLD, KC_VOLU,   RGB_TOG,      NUMX,  _______,  _______,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,         KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,   KC_MINS,   KC_EQL,  KC_BSPC,   _______,  _______,  _______,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,         KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,   KC_LBRC,  KC_RBRC,  KC_BSLS,   DM_PLY1,  DM_PLY2,  _______,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,         KC_G,     KC_H,     KC_J,     KC_K,     KC_L,  KC_SCLN,   KC_QUOT,   KC_ENT,
        KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,         KC_B,     KC_N,     KC_M,  KC_COMM,   KC_DOT,  KC_SLSH,   KC_RSFT,                                   KC_UP,
        KC_LCTL,  KC_LGUI,  KC_LALT,                        KC_SPC,                                          KC_RALT,   KC_RGUI, MO(WIN_FN), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [WIN_FN] = LAYOUT_tkl_ansi(
        QK_BOOT,              KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,    KC_F12,  RGB_MODX,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,   _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,   _______,  DM_REC1,  DM_REC2,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,                                  RGB_VAIX,
        _______,  _______,  _______,                      _______,                                         TG(GAME), TG(GAME),    _______,  _______, RGB_RMODX, RGB_VADX, RGB_MODX),

    [GAME] = LAYOUT_tkl_ansi(
        _______,            _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,   _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,     KC_1,     KC_2,     KC_3,     KC_4,       KC_5,   _______,  _______,  _______,  _______,
        _______,  _______,     KC_O,  _______,  _______,  _______,  _______,     KC_R,     KC_E,     KC_W,  _______,   KC_TAB,    _______,   _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,     KC_F,     KC_A,     KC_S,     KC_D,  _______,    _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_LCTL,  KC_LSFT,                                      KC_C,
        _______,    KC_NO,  _______,                      _______,                                          _______,  KC_LCTL,    _______,   _______,     KC_Z,     KC_X,     KC_V),

    [NP_FUN] = LAYOUT_tkl_ansi(
        _______,            _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,   _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,   _______,   KC_INS,  KC_HOME,  KC_PGUP,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,   _______,   KC_DEL,   KC_END,  KC_PGDN,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,                                   _______,
        _______,  _______,  _______,                      _______,                                          _______, _______,     _______,   _______,  _______,  _______,  _______)
};

/*
    [BLANK] = LAYOUT_tkl_ansi(
        _______,            _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,   _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,   _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,   _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,                                   _______,
        _______,  _______,  _______,                      _______,                                          _______, _______,     _______,   _______,  _______,  _______,  _______)
 */

bool dip_switch_update_user(uint8_t index, bool active){
  switch(index){
    case 0:
    //   if(active) { // Mac mode
    //       layer_move(MAC_BASE);
    //   } else { // Windows mode
    //       layer_move(WIN_BASE);
    //   }
      return false;
    case 1:
      if(active){ //Cable mode
        // do stuff
      }
      else{ //BT mode
        // do stuff
      }
      break;
  }

  return true;
}

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

  keys_color[0] = pgm_read_byte(&COLORS[user_config.keys_color][0]);
  keys_color[1] = pgm_read_byte(&COLORS[user_config.keys_color][1]);
  keys_color[2] = pgm_read_byte(&COLORS[user_config.keys_color][2]);

  mods_color[0] = pgm_read_byte(&COLORS[user_config.mods_color][0]);
  mods_color[1] = pgm_read_byte(&COLORS[user_config.mods_color][1]);
  mods_color[2] = pgm_read_byte(&COLORS[user_config.mods_color][2]);

  pads_color[0] = pgm_read_byte(&COLORS[user_config.pads_color][0]);
  pads_color[1] = pgm_read_byte(&COLORS[user_config.pads_color][1]);
  pads_color[2] = pgm_read_byte(&COLORS[user_config.pads_color][2]);
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
        for (uint8_t i = 0; i < RGB_GROUP_PAD_NUMLOCK_SIZE; i++) {
            //rgb_matrix_set_color(RGB_GROUP_PAD[i], rgb_pads_color.r, rgb_pads_color.g, rgb_pads_color.b);
            rgb_matrix_set_color(RGB_GROUP_PAD_NUMLOCK[i], 0, 0, 0);
        }

        if (IS_LAYER_ON(GAME)) {
            RGB game = hsv_to_rgb_keep_brightness(HSV_RED);
            rgb_matrix_set_color(42, game.r, game.g, game.b);
            rgb_matrix_set_color(58, game.r, game.g, game.b);
            rgb_matrix_set_color(59, game.r, game.g, game.b);
            rgb_matrix_set_color(60, game.r, game.g, game.b);
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
            RGB numpad = hsv_to_rgb_keep_brightness(HSV_RED);
            for (uint8_t i = 0; i < RGB_GROUP_PAD_NUMLOCK_SIZE; i++) {
                rgb_matrix_set_color(RGB_GROUP_PAD_NUMLOCK[i], numpad.r, numpad.g, numpad.b);
            }
        } else {
            /*
             * Macropad area
             */
            if (dynamic_macro_is_recording(1)) {
                RGB color = hsv_to_rgb_keep_brightness(HSV_RED);
                rgb_matrix_set_color(47, color.r, color.g, color.b);
            }
            else if (dynamic_macro_is_recorded(1)) {
                RGB color = hsv_to_rgb_keep_brightness(HSV_YELLOW);
                rgb_matrix_set_color(47, color.r, color.g, color.b);
            }

            if (dynamic_macro_is_recording(-1)) {
                RGB color = hsv_to_rgb_keep_brightness(HSV_RED);
                rgb_matrix_set_color(48, color.r, color.g, color.b);
            }
            else if (dynamic_macro_is_recorded(-1)) {
                RGB color = hsv_to_rgb_keep_brightness(HSV_YELLOW);
                rgb_matrix_set_color(48, color.r, color.g, color.b);
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
            RGB color = hsv_to_rgb_keep_brightness(HSV_YELLOW);
            rgb_matrix_set_color(15, color.r, color.g, color.b);
        } else {
            RGB color = hsv_to_rgb_keep_brightness(HSV_BLUE);
            rgb_matrix_set_color(15, color.r, color.g, color.b);
        }
    }
    return true;
}
