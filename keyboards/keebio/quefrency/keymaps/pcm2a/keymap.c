// Copyright 2024 Danny Nguyen (@nooges)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "transactions.h"
#include "print.h"

#define MACRO_IDS {39, 40, 41, 42, 43, 44, 45, 46, 47, 49}
#define UG_IDS {1, 4, 8, 27, 33, 36, 39, 48, 51, 55, 58, 61, 87, 92, 96, 100}
// #define UG_LEFT_IDS {1, 4, 8, 27, 33, 36, 39, 48}
// #define UG_RIGHT_IDS {51, 55, 58, 61, 87, 92, 96, 100}

static const uint8_t UG_LEFT_IDS[] = {1, 4, 8, 27, 33, 36, 39, 48};
static const uint8_t UG_RIGHT_IDS[] = {51, 55, 58, 61, 87, 92, 96, 100};

enum custom_keycodes {
    COL_KEYS = SAFE_RANGE,
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
    GO_KEY
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
    uint8_t keys_color[3];
} synced_data_t;

synced_data_t synced_data = {0, {0, 0, 0}};

bool synced_update = false;
uint32_t synced_time = 0;

const uint8_t COLORS_SIZE = 12;
const uint8_t PROGMEM COLORS[][3] = { {HSV_BLUE}, {HSV_WHITE}, {HSV_YELLOW}, {HSV_RED}, {HSV_GREEN}, {HSV_ORANGE}, {HSV_AZURE}, {HSV_CORAL}, {HSV_CYAN}, {HSV_GOLDENROD}, {HSV_MAGENTA}, {HSV_PURPLE} };
const uint8_t DEFAULT_KEYS_COLOR = 4;

static uint8_t current_led = 0;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_all(
    KC_MUTE, KC_ESC,  KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_NO,   KC_BSPC, KC_HOME,
    KC_F3,   KC_F4,   KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_END,
    KC_F5,   KC_F6,   KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_NUHS, KC_ENT,  KC_PGUP,
    KC_F7,   KC_F8,   KC_LSFT, KC_NUBS, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_UP,   KC_PGDN,
    KC_F9,   KC_F10,  KC_LCTL, KC_LGUI, KC_LALT, MO(1),   KC_SPC,  KC_SPC,  KC_SPC,  KC_SPC,  KC_RALT, MO(1),   KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
  ),

  [1] = LAYOUT_all(
    _______, QK_BOOT, QK_GESC, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,  KC_BSPC, QK_BOOT,
    COL_KEYS, GO_KEY, RGB_TOGX, RGB_MODX, _______, KC_UP,   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, GO_KEY,
    RGB_VAIX, RGB_VADX, _______, _______, KC_LEFT, KC_DOWN, KC_RGHT, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    RGB_HUIX, RGB_HUDX, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    RGB_SAIX, RGB_SADX, KC_TILD, _______, TG(2), _______, _______, _______, _______, _______, TG(2), _______, _______, _______, _______, RESET
  ),

  [2] = LAYOUT_all(
    _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    _______, KC_B,    _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_R,    KC_E,    KC_W,    KC_Q,    KC_TAB,  KC_B, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, KC_G,    KC_F,    KC_A,    KC_S,    KC_D,    _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_M,    KC_X,    KC_V,    KC_LCTL, KC_LSFT, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, MO(1),   KC_LCTL, _______, _______, _______
  ),

  [3] = LAYOUT_all(
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
  )
};

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [1] = { ENCODER_CCW_CW(UG_PREV, UG_NEXT), ENCODER_CCW_CW(KC_MPRV, KC_MNXT) },
    [2] = { ENCODER_CCW_CW(UG_HUED, UG_HUEU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [3] = { ENCODER_CCW_CW(UG_SPDD, UG_SPDU), ENCODER_CCW_CW(UG_VALD, UG_VALU) },
};
#endif

void sync_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) {
    print("debug 1\n");
    if (in_buflen == sizeof(synced_data_t)) {
        print("debug 2\n");
        synced_data_t *received_data = (synced_data_t *) in_data;
        if (received_data->timer > 0) {
            synced_data.timer = received_data->timer;
            synced_update = true;

            synced_data.keys_color[0] = received_data->keys_color[0];
            synced_data.keys_color[1] = received_data->keys_color[1];
            synced_data.keys_color[2] = received_data->keys_color[2];
            dprintf("Got %d %d %d\n", synced_data.keys_color[0], synced_data.keys_color[1], synced_data.keys_color[2]);
        }
    }
}

void keyboard_post_init_user(void) {

    // debug_enable=true;

    transaction_register_rpc(RPC_SYNC, sync_handler);

  // Should load from eeprom here...
  user_config.raw = 0;
  user_config.keys_color = DEFAULT_KEYS_COLOR;

  synced_data.keys_color[0] = pgm_read_byte(&COLORS[user_config.keys_color][0]);
  synced_data.keys_color[1] = pgm_read_byte(&COLORS[user_config.keys_color][1]);
  synced_data.keys_color[2] = pgm_read_byte(&COLORS[user_config.keys_color][2]);

//   rgb_matrix_sethsv(85, 255, 255);
//   rgb_matrix_sethsv_noeeprom(85, 255, 255);
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
      synced_data.keys_color[0] = pgm_read_byte(&COLORS[user_config.keys_color][0]);
      synced_data.keys_color[1] = pgm_read_byte(&COLORS[user_config.keys_color][1]);
      synced_data.keys_color[2] = pgm_read_byte(&COLORS[user_config.keys_color][2]);

      synced_data.timer = sync_timer_read();

      if (transaction_rpc_send(RPC_SYNC, sizeof(synced_data), &synced_data)) {
        print("Sync sent\n");
      }

    } else {
      rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
    }
  }
}

void cycle_underglow_colors(const uint8_t led_ids[], size_t num_leds) {
    // uint8_t led_ids[] = UG_IDS;
    // uint8_t num_leds = sizeof(led_ids) / sizeof(led_ids[0]);

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

// void cycle_underglow_colors_left(void) {
//     uint8_t led_ids[] = UG_LEFT_IDS;
//     uint8_t num_leds = sizeof(led_ids) / sizeof(led_ids[0]);

//     uint8_t saturation = 255;
//     uint8_t brightness = rgb_matrix_get_val(); // Get the current brightness level

//     // Use g_rgb_timer to keep both halves in sync
//     uint32_t timer_val = g_rgb_timer;
//     uint16_t hue = (timer_val / 100) % 256; // Adjust the division factor to change speed

//     // Cycle through colors with the defined speed
//     for (uint8_t i = 0; i < num_leds; i++) {
//         uint8_t led_index = led_ids[i];
//         RGB rgb = hsv_to_rgb((HSV){ .h = hue, .s = saturation, .v = brightness });
//         rgb_matrix_set_color(led_index, rgb.r, rgb.g, rgb.b);
//     }
// }

// void cycle_underglow_colors_right(void) {
//     uint8_t led_ids[] = UG_RIGHT_IDS;
//     uint8_t num_leds = sizeof(led_ids) / sizeof(led_ids[0]);

//     uint8_t saturation = 255;
//     uint8_t brightness = rgb_matrix_get_val(); // Get the current brightness level

//     // Use g_rgb_timer to keep both halves in sync
//     uint32_t timer_val = g_rgb_timer;
//     uint16_t hue = (timer_val / 100) % 256; // Adjust the division factor to change speed

//     // Cycle through colors with the defined speed
//     for (uint8_t i = 0; i < num_leds; i++) {
//         uint8_t led_index = led_ids[i];
//         RGB rgb = hsv_to_rgb((HSV){ .h = hue, .s = saturation, .v = brightness });
//         rgb_matrix_set_color(led_index, rgb.r, rgb.g, rgb.b);
//     }
// }

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  if (record->event.pressed) {

    synced_data.timer = sync_timer_read();
    // if (transaction_rpc_send(RPC_SYNC, sizeof(synced_data), &synced_data)) {
    //     // print("Sync sent\n");
    // }

    switch (keycode) {

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

        case GO_KEY:
        print("hello");
            current_led++;
            if (current_led >= RGB_MATRIX_LED_COUNT) {
                current_led = 0; // Loop back to the first LED
            }
            dprintf("Current LED: %d, Max LED: %d\n", current_led, RGB_MATRIX_LED_COUNT);
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

void set_game_leds(void) {
    uint8_t leds[] = {68, 74, 75, 76};
    // uint8_t leds[] = {18, 24, 25, 26};
    for (uint8_t i = 0; i < ARRAY_SIZE(leds); i++) {
        HSV hsv = { 0, 255, rgb_matrix_get_val() };
        RGB rgb = hsv_to_rgb(hsv);
        rgb_matrix_set_color(leds[i], rgb.r, rgb.g, rgb.b);
    }
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {

    uint32_t mode = rgblight_get_mode();
    // assign colors if the matrix is on and the current mode
    // is SOLID COLORS => No animations running
    if(rgb_matrix_config.enable == 1 && mode == 1) { // && swap_colors) {

        // RGB rgb_color = hsv_to_rgb_keep_brightness(synced_data.keys_color[0], synced_data.keys_color[1], synced_data.keys_color[2]);

        for (uint8_t i = led_min; i < led_max; i++) {
            // rgb_matrix_set_color(i, rgb_color.r, rgb_color.g, rgb_color.b);
            // rgb_matrix_set_color(i, 0,0,0);

            // if (i < 10) {
            //     rgb_matrix_set_color(i, COLORS[0][0], COLORS[0][1], COLORS[0][2]);
            // }
        }

        // rgb_matrix_set_color(current_led, 255, 255, 255);

        // cycle_underglow_colors();

        if (is_keyboard_left()) {
            rgb_matrix_set_color(31, 0,0,0);
            cycle_underglow_colors(UG_LEFT_IDS, 8);
        } else {
            rgb_matrix_set_color(89, 0,0,0);
            cycle_underglow_colors(UG_RIGHT_IDS, 8);
        }

        // uint8_t led_ids[] = UG_IDS;
        // uint8_t num_leds = sizeof(led_ids) / sizeof(led_ids[0]);
        // for (uint8_t i = 0; i < num_leds; i++) {
        //     rgb_matrix_set_color(led_ids[i], 255, 255, 255);
        // }

        if (IS_LAYER_ON(2)) {
            set_game_leds();
        }
    }
    return true;
}
