 /* Copyright 2020 Josef Adamcik
  * Modification for VIA support and RGB underglow by Jens Bonk-Wiltfang
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
#include "starfield.h"
#include "transactions.h"

// Define the LEDs for each group
// #define LEFT_LEDS {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35}
// #define RIGHT_LEDS {36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71}

// Underglow leds
#define LED_IDS {1, 2, 3, 4, 5, 6, 37, 38, 39, 40, 41, 42}

// Tap Dance Declarations
enum {
  TD_ONE_ESC = 0,
  TD_GRV_CAPS = 1
};

//Tap Dance Definitions
tap_dance_action_t tap_dance_actions[] = {
  [TD_ONE_ESC] = ACTION_TAP_DANCE_DOUBLE(KC_1, KC_ESC),
  [TD_GRV_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_GRV, KC_CAPS)
};

enum custom_keycodes {
    COL_KEYS = SAFE_RANGE,
    RGB_TOGX,
    RRGB_TOGX,
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
};
typedef union {
  uint32_t raw;
  struct {
    uint8_t keys_color;
  };
} user_config_t;

user_config_t user_config;

typedef struct {
    uint32_t timer;
} synced_data_t;

synced_data_t synced_data = {0};

bool synced_update = false;
uint32_t synced_time = 0;

const uint8_t COLORS_SIZE = 12;
const uint8_t PROGMEM COLORS[][3] = { {HSV_BLUE}, {HSV_WHITE}, {HSV_YELLOW}, {HSV_RED}, {HSV_GREEN}, {HSV_ORANGE}, {HSV_AZURE}, {HSV_CORAL}, {HSV_CYAN}, {HSV_GOLDENROD}, {HSV_MAGENTA}, {HSV_PURPLE} };
const uint8_t DEFAULT_KEYS_COLOR = 1;

uint8_t keys_color[3];

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { ENCODER_CCW_CW(KC_VOLU, KC_VOLD),           ENCODER_CCW_CW(KC_BRIU, KC_BRID) },
    [1] = { ENCODER_CCW_CW(_______, _______),           ENCODER_CCW_CW(_______, _______) },
    [2] = { ENCODER_CCW_CW(RGB_HUDX, RGB_HUIX),           ENCODER_CCW_CW(RGB_SADX, RGB_SAIX) },
    [3] = { ENCODER_CCW_CW(RGB_VAIX, RGB_VADX),           ENCODER_CCW_CW(RGB_MODX, RGB_RMODX)},
    [4] = { ENCODER_CCW_CW(KC_VOLU, KC_VOLD),           ENCODER_CCW_CW(KC_BRIU, KC_BRID) },
    [5] = { ENCODER_CCW_CW(_______, _______),           ENCODER_CCW_CW(_______, _______) },
    // [3] = { ENCODER_CCW_CW(LFT_VAI, LFT_VAD),           ENCODER_CCW_CW(RGT_VAI, RGT_VAD)}
};
#endif

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * QWERTY
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |  `   |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  `   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | ESC  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  | Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   A  |   S  |   D  |   F  |   G  |-------.    ,-------|   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
 * |LShift|   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  |   /  |RShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |LOWER | /Enter  /       \Space \  |RAISE | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */

[0] = LAYOUT(
  TD(TD_ONE_ESC),   KC_2,   KC_3,    KC_4,    KC_5,    KC_6,                     KC_7,    KC_8,    KC_9,    KC_0, KC_MINS,   KC_BSPC,
   KC_TAB,  KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,  KC_SLSH,
  TD(TD_GRV_CAPS),  KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                     KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN,  KC_QUOT,
  KC_LSFT,  KC_Z,   KC_X,    KC_C,    KC_V,    KC_B, KC_MUTE,    XXXXXXX,KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  KC_RSFT,
                 KC_LGUI,KC_LCTL,KC_LALT, KC_SPC, MO(2),      LT(3, KC_ENT),  KC_SPC, KC_EQL, KC_LBRC, KC_RBRC
),
/*
 * COLEMAK
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |  `   |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  `   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | ESC  |   Q  |   W  |   F  |   P  |   G  |                    |   J  |   L  |   U  |   Y  |   ;  | Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | TAB  |   A  |   R  |   S  |   T  |   D  |-------.    ,-------|   H  |   N  |   E  |   I  |   O  |  '   |
 * |------+------+------+------+------+------|  MUTE |    |       |------+------+------+------+------+------|
 * |LShift|   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   K  |   M  |   ,  |   .  |   /  |RShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |LOWER | /Enter  /       \Space \  |RAISE | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `-----------------------------------'           '------''---------------------------'
 */

[1] = LAYOUT(
  KC_GRV,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  KC_GRV,
  KC_ESC,   KC_Q,   KC_W,    KC_F,    KC_P,    KC_G,                      KC_J,    KC_L,    KC_U,    KC_Y, KC_SCLN,  KC_BSPC,
  KC_TAB,   KC_A,   KC_R,    KC_S,    KC_T,    KC_D,                      KC_H,    KC_N,    KC_E,    KC_I,    KC_O,  KC_QUOT,
  KC_LSFT,  KC_Z,   KC_X,    KC_C,    KC_V,    KC_B, KC_MUTE,      XXXXXXX,KC_K,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  KC_RSFT,
                 KC_LGUI,KC_LALT,KC_LCTL,KC_TRNS, KC_ENT,      KC_SPC,  KC_TRNS, KC_RCTL, KC_RALT, KC_RGUI
),
/* LOWER
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |  F1  |  F2  |  F3  |  F4  |  F5  |                    |  F6  |  F7  |  F8  |  F9  | F10  | F11  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  `   |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  | F12  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   !  |   @  |   #  |   $  |   %  |-------.    ,-------|   ^  |   &  |   *  |   (  |   )  |   |  |
 * |------+------+------+------+------+------|  MUTE |    |       |------+------+------+------+------+------|
 * | Shift|  =   |  -   |  +   |   {  |   }  |-------|    |-------|   [  |   ]  |   ;  |   :  |   \  | Shift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |LOWER | /Enter  /       \Space \  |RAISE | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[2] = LAYOUT(
  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                       KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
  KC_GRV,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                       KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  KC_F12,
  _______, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,                       KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_PIPE,
  _______,  KC_EQL, KC_MINS, KC_PLUS, KC_LCBR, KC_RCBR, _______,      RGB_TOGX, KC_LBRC, KC_RBRC, KC_UP, KC_COLN, KC_BSLS, _______,
                       QK_CLEAR_EEPROM, _______, _______, TG(4), _______,       _______, _______, KC_LEFT, KC_DOWN, KC_RIGHT
),
/* RAISE
 * ,----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Esc  | Ins  | Pscr | Menu |      |      |                    |      | PWrd |  Up  | NWrd | DLine| Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  | LAt  | LCtl |LShift|      | Caps |-------.    ,-------|      | Left | Down | Rigth|  Del | Bspc |
 * |------+------+------+------+------+------|  MUTE  |   |       |------+------+------+------+------+------|
 * |Shift | Undo |  Cut | Copy | Paste|      |--------|   |-------|      | LStr |      | LEnd |      | Shift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |LOWER | /Enter  /       \Space \  |RAISE | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[3] = LAYOUT(
  _______, _______ , _______ , _______ , _______ , _______,                           _______,  _______  , _______,  _______ ,  _______ ,_______,
  _______,  KC_INS,  KC_PSCR,   KC_APP,  XXXXXXX, XXXXXXX,                        KC_PGUP, _______,   KC_UP, _______,_______, KC_BSPC,
  _______, KC_LALT,  KC_LCTL,  KC_LSFT,  XXXXXXX, KC_CAPS,                       KC_PGDN,  KC_LEFT, KC_DOWN, KC_RGHT,  KC_DEL, KC_BSPC,
  _______,KC_UNDO, KC_CUT, KC_COPY, KC_PASTE, XXXXXXX,  _______,       _______,  XXXXXXX, _______, XXXXXXX, _______,   XXXXXXX, _______,
                         _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______
),
/* GAME
 * ,----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |   1  |   2  |   3  |   4  |   5  |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |                    |   R  |   E  |   W  |      | TAB  |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------.    ,-------|   F  |   A  |   S  |   D  |LShft | LShft|
 * |------+------+------+------+------+------|  MUTE  |   |       |------+------+------+------+------+------|
 * |      |      |      |      |      |      |--------|   |-------|   Z  |   X  |   C  |   V  |LCtrl | LCtrl|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            |      |      |      |      | /Enter  /       \Space \  |Space | Space|      |      |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[4] = LAYOUT(
  _______, _______ , _______ , _______ , _______ , _______,                           _______,  KC_1  , KC_2,  KC_3 ,  KC_4 , KC_5,
  _______,  _______,  _______,   _______,  _______, _______,                        KC_R, KC_E,   KC_W, KC_NO, KC_TAB, KC_NO,
  _______, _______,  _______,  _______,  _______, _______,                          KC_F,  KC_A, KC_S, KC_D, KC_LSFT, KC_LSFT,
  _______,_______, _______, _______, _______, _______,  _______,       _______,     KC_M, KC_Z, KC_X, KC_C,   KC_V, KC_LCTL,
                         _______, _______, _______, _______, _______,       KC_SPC, KC_SPC, KC_SPC, _______, _______
),
/* Arrow Keys
 * ,----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |   1  |   2  |   3  |   4  |   5  |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |                    |   R  |   E  |   W  |      | TAB  |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------.    ,-------|   F  |   A  |   S  |   D  |LShft | LShft|
 * |------+------+------+------+------+------|    |   |       |------+------+------+------+------+------|
 * |      |      |      |      |      |      |--------|   |-------|   Z  |   X  |   C  |   V  |LCtrl | LCtrl|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            |      |      |      |      | /       /       \      \  |      |      |      |      |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[5] = LAYOUT(
  _______, _______ , _______ , _______ , _______ , _______,                           _______,  _______  , _______,  _______ ,  _______ , _______,
  _______,  _______,  _______,   _______,  _______, _______,                        _______, _______,   _______, _______, _______, _______,
  _______, _______,  _______,  _______,  _______, _______,                       _______,  _______, _______, _______,  _______, _______,
  _______,_______, _______, _______, _______, _______,  _______,       _______,  _______, _______, KC_UP, _______, _______, _______,
                         _______, _______, _______, _______, _______,       _______, _______, KC_LEFT, KC_DOWN, KC_RIGHT
)
};

void oled_sync_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) {
    if (in_buflen == sizeof(synced_data_t)) {
        synced_data_t *received_data = (synced_data_t *) in_data;
        synced_data.timer = received_data->timer;
        synced_update = true;
    }
}

void keyboard_post_init_user(void) {

    transaction_register_rpc(RPC_OLED_SYNC, oled_sync_handler);

  // Customise these values to desired behaviour
  //debug_enable=true;
  //debug_matrix=true;
  //debug_keyboard=true;
  //debug_mouse=true;

  // Should load from eeprom here...
  user_config.raw = 0;
  user_config.keys_color = DEFAULT_KEYS_COLOR;

  keys_color[0] = pgm_read_byte(&COLORS[user_config.keys_color][0]);
  keys_color[1] = pgm_read_byte(&COLORS[user_config.keys_color][1]);
  keys_color[2] = pgm_read_byte(&COLORS[user_config.keys_color][2]);

  init_stars();
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

void cycle_underglow_colors(void) {
    uint8_t led_ids[] = LED_IDS;
    uint8_t num_leds = sizeof(led_ids) / sizeof(led_ids[0]);

    uint8_t saturation = 255;
    uint8_t brightness = rgb_matrix_get_val(); // Get the current brightness level

    // Use g_rgb_timer to keep both halves in sync
    uint32_t timer_val = g_rgb_timer;
    uint16_t hue = (timer_val / 100) % 256; // Adjust the division factor to change speed

    // Cycle through colors with the defined speed
    for (uint8_t i = 0; i < num_leds; i++) {
        uint8_t led_index = led_ids[i];
        RGB rgb = hsv_to_rgb((HSV){ .h = hue, .s = saturation, .v = brightness });
        rgb_matrix_set_color(led_index, rgb.r, rgb.g, rgb.b);
    }
}

RGB hsv_to_rgb_keep_brightness(int h, int s, int v) {
    HSV hsv = { h, s, v };
    if (hsv.v > rgb_matrix_get_val()) {
        hsv.v = rgb_matrix_get_val();
    }
    return hsv_to_rgb(hsv);
}

void turn_down_layer_leds(void) {
    uint8_t leds[] = {0, 36};
    HSV hsv = { rgb_matrix_get_hue(), rgb_matrix_get_sat(), 30};
    RGB rgb = hsv_to_rgb(hsv);
    for (uint8_t i = 0; i < ARRAY_SIZE(leds); i++) {
        rgb_matrix_set_color(leds[i], rgb.r, rgb.g, rgb.b);
    }
}

void set_game_leds(void) {
    uint8_t leds[] = {55, 59, 60, 65};
    for (uint8_t i = 0; i < ARRAY_SIZE(leds); i++) {
        HSV hsv = { 0, 255, rgb_matrix_get_val() };
        RGB rgb = hsv_to_rgb(hsv);
        rgb_matrix_set_color(leds[i], rgb.r, rgb.g, rgb.b);
    }
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {

    if(rgb_matrix_config.enable == 1) {

        cycle_underglow_colors();

        turn_down_layer_leds();

        if (IS_LAYER_ON(4)) {
            set_game_leds();
        }
    }

    return false;
}

// bool rgb_matrix_indicators_user(void) {
//     // Iterate through all LEDs
//     for (uint8_t i = 0; i < 72; i++) {
//         // Set each LED to HSV_WHITE
//         RGB rgb = hsv_to_rgb_keep_brightness((HSV) { .h = 0, .s = 0, .v = 255 });
//         rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
//     }
//     return false;
// }

// bool rgb_matrix_indicators_user(void) {
//     // Iterate through all LEDs
//     for (uint8_t i = 0; i < 72; i++) {
//         // if (i >= 0 && i <= 35) {
//         //     // Turn off LEDs 0-35
//         //     rgb_matrix_set_color(i, 0, 0, 0);
//         // } else if (i >= 36 && i <= 71) {
//         //     // Set LEDs 36-71 to white
//         //     RGB rgb = hsv_to_rgb((HSV) { .h = 0, .s = 0, .v = 255 });
//         //     rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
//         // }
//         rgb_matrix_set_color(i, 0, 0, 0);
//         if ((i >= 16 && i <= 19) || (i >= 46 && i <= 49)) {
//             RGB rgb = hsv_to_rgb((HSV) { .h = 0, .s = 0, .v = 255 });
//             rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
//         }
//     }
//     return false;
// }

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  // static bool shift_pressed = false;
  // static bool alt_pressed = false;

  if (record->event.pressed) {

    synced_data.timer = sync_timer_read();
    if (transaction_rpc_send(RPC_OLED_SYNC, sizeof(synced_data), &synced_data)) {
        // print("Sync sent\n");
    }

    switch (keycode) {

      // case KC_LSFT:
      //       if (record->event.pressed) {
      //           shift_pressed = true;
      //       } else {
      //           shift_pressed = false;
      //       }
      //       break;

      //   case KC_LALT:
      //       if (record->event.pressed) {
      //           alt_pressed = true;
      //       } else {
      //           alt_pressed = false;
      //       }
      //       break;

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

      case COL_KEYS:
        cycle_keys_color();
        break;
    }

    // if (shift_pressed && alt_pressed) {
    //     layer_on(5);
    // } else {
    //     layer_off(5);
    // }
  }
  return true;
}

void matrix_scan_user(void) {

    static bool shift_pressed = false;
    static bool alt_pressed = false;

    // Check if both keys are pressed
    bool current_shift = get_mods() & MOD_BIT(KC_LSFT);
    bool current_alt = get_mods() & MOD_BIT(KC_LALT);

    if (current_shift && current_alt) {
        if (!shift_pressed || !alt_pressed) {
            // Both were not pressed before, so toggle the layer
            layer_on(5);
        }
    } else {
        if (shift_pressed && alt_pressed) {
            // Both were pressed before, so turn off the layer
            layer_off(5);
        }
    }

    shift_pressed = current_shift;
    alt_pressed = current_alt;
}

// static void render_logo(void) {
//     static const char PROGMEM qmk_logo[] = {
//         0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
//         0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
//         0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0
//     };
//     oled_write_P(qmk_logo, false);
// }

static void render_logo_ergomech(void) {
    static const char PROGMEM logo[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x60, 0xF0, 0xF0, 0x38, 0x38,
    0x38, 0x3C, 0x3C, 0x3C, 0x3C, 0x3E,
    0xFE, 0xFE, 0x3E, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xE0, 0xF8, 0xFC, 0x1C,
    0xDC, 0x98, 0x3E, 0x7F, 0x7F, 0xFB,
    0xDB, 0x9F, 0x8F, 0x06, 0x06, 0x8F,
    0xCB, 0xFB, 0xFF, 0xBF, 0x3F, 0x78,
    0xF8, 0xF0, 0x80, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
    0xE0, 0xE0, 0xE0, 0xE0, 0xC0, 0x00,
    0xE0, 0xE0, 0x60, 0x60, 0xC0, 0xE0,
    0x60, 0x60, 0xE0, 0xE0, 0x00, 0xC0,
    0xE0, 0x60, 0x60, 0x60, 0xC0, 0x80,
    0xE0, 0xE0, 0x60, 0x60, 0xE0, 0xE0,
    0x60, 0x60, 0xE0, 0x00, 0xC0, 0xE0,
    0xE0, 0xE0, 0xE0, 0xC0, 0x00, 0xC0,
    0xE0, 0x60, 0x60, 0x60, 0x40, 0xFC,
    0xFC, 0x60, 0x60, 0xE0, 0x80, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xC0,
    0xC0, 0xE0, 0xF0, 0xB0, 0xFF, 0xFF,
    0xFF, 0xFC, 0xF3, 0xFF, 0xFE, 0xF8,
    0xE0, 0x00, 0x01, 0x03, 0x01, 0x01,
    0x81, 0x81, 0xE0, 0xFE, 0xFF, 0xFB,
    0xF0, 0xFF, 0xFF, 0xFF, 0xE3, 0xE0,
    0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00,
    0x00, 0x03, 0x07, 0x0D, 0x0D, 0x0D,
    0x07, 0x01, 0x0F, 0x0F, 0x00, 0x00,
    0x37, 0x36, 0x6C, 0x6C, 0x36, 0x3F,
    0x00, 0x07, 0x0F, 0x0C, 0x0C, 0x0E,
    0x07, 0x03, 0x0F, 0x0F, 0x00, 0x00,
    0x0F, 0x07, 0x00, 0x00, 0x0F, 0x00,
    0x03, 0x07, 0x0D, 0x0D, 0x0D, 0x07,
    0x00, 0x07, 0x0F, 0x0C, 0x0C, 0x0C,
    0x04, 0x0F, 0x07, 0x00, 0x00, 0x0F,
    0x07, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x10,
    0x30, 0x3F, 0x33, 0x33, 0x33, 0x33,
    0x33, 0x33, 0x33, 0x33, 0x33, 0x3F,
    0x33, 0x3F, 0x33, 0x11, 0x30, 0x70,
    0x30, 0x71, 0x73, 0x7F, 0x77, 0x7F,
    0x77, 0x77, 0x77, 0x77, 0x77, 0x77,
    0x33, 0x33, 0x33, 0x33, 0x3F, 0x30,
    0x30, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,
};
    oled_write_raw_P(logo, sizeof(logo));
}

void print_status_stuff(void) {
    oled_write_P(PSTR("\n\n"), false);
    switch (get_highest_layer(layer_state)) {
        case 0:
            oled_write_ln_P(PSTR("Qwrt"), false);
            break;
        case 1:
            oled_write_ln_P(PSTR("Clmk"), false);
            break;
        default:
            oled_write_P(PSTR("Mod\n"), false);
            break;
    }
    oled_write_P(PSTR("\n\n"), false);
    oled_write_ln_P(PSTR("LAYER"), false);
    switch (get_highest_layer(layer_state)) {
        case 0:
        case 1:
            oled_write_P(PSTR("Base\n"), false);
            break;
        case 2:
            oled_write_P(PSTR("Raise\n"), false);
            break;
        case 3:
            oled_write_P(PSTR("Lower\n"), false);
            break;
        case 4:
            oled_write_P(PSTR("Game\n"), false);
            break;
        case 5:
            oled_write_P(PSTR("Arrow\n"), false);
            break;
        default:
            oled_write_ln_P(PSTR("Undef\n"), false);
    }
    oled_write_P(PSTR("\n\n"), false);
    led_t led_usb_state = host_keyboard_led_state();
    oled_write_ln_P(PSTR("CPSLK"), led_usb_state.caps_lock);
}

void print_right_side(void) {
    oled_write_P(PSTR("\n\n"), false);

    if (synced_update) {
        synced_time = timer_read32();
        synced_update = false;
    }
    if (synced_time > 0) {
        uint32_t seconds = timer_elapsed32(synced_time) / 1000;
        if (seconds < 60) {
            char buf[16];
            snprintf(buf, sizeof(buf), "%lu\n", seconds);
            oled_write_P(buf, false);
        } else {
            oled_write_P(PSTR("Done\n"), false);
        }
    } else {
        oled_write_P(PSTR("None\n"), false);
    }

    // static bool animation_clear_once = false;
    // static bool text_clear_once = false;

    // if (synced_update) {
    //     synced_time = timer_read32();
    //     synced_update = false;
    // }
    // if (synced_time > 0) {
    //     uint32_t seconds = timer_elapsed32(synced_time) / 1000;
    //     if (seconds < 30) {

    //         if (text_clear_once) {
    //             oled_clear();
    //             text_clear_once = false;
    //         }
    //         animation_clear_once = true;

    //         static uint32_t last_update = 0;
    //         if (timer_elapsed32(last_update) > 100) { // Update every 100ms
    //             last_update = timer_read32();
    //             update_and_render_stars();
    //         }
    //     } else if (seconds < 60) {

    //         if (animation_clear_once) {
    //             oled_clear();
    //             animation_clear_once = false;
    //         }
    //         text_clear_once = true;

    //         char buf[16];
    //         snprintf(buf, sizeof(buf), "%lu\n", seconds);
    //         oled_write_P(buf, false);
    //     } else {
    //         oled_write_P(PSTR("Done\n"), false);
    //     }
    // }
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        // render_logo_ergomech();
        print_status_stuff();
    } else {
        render_logo_ergomech();
        // print_status_stuff();
        // print_right_side();
        // render_logo();
        // static uint32_t last_update = 0;
        // if (timer_elapsed32(last_update) > 100) { // Update every 100ms
        //     last_update = timer_read32();
        //     update_and_render_stars();
        // }
    }
    return false;
}
