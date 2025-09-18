// Copyright 2025 EPOMAKER (@Epomaker)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
//#include "rgb_record/rgb_record.h"

static const int LED_TOTAL = 68;
static const uint8_t GAME_LED_IDS[] = {41, 47, 48, 49, /* these are o k l ; */ 36, 52, 53 /* these are end, pgup, pgdn */};

enum layers {
    _BL = 0,
    _FL,
    _MBL,
    _MFL,
    GAME,
};

enum custom_keycodes {
    LEDTEST = SAFE_RANGE,
};

static uint8_t led_index = 0;

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_BL] = LAYOUT( /* Base */
        QK_GESC,  KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,           KC_MUTE,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,               KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS, KC_END,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,               KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,  KC_ENT,            KC_PGUP,
        KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,               KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,  KC_UP,   KC_PGDN,
        KC_LCTL,  KC_LCMD,  KC_LALT,  KC_SPC,                                 KC_SPC,   KC_RALT,  MO(_FL),  KC_RCTL,                      KC_LEFT,  KC_DOWN, KC_RGHT),

    [_FL] = LAYOUT( /* Base */
        KC_GRV,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,              KC_F6,    KC_F7,    KC_F8,    KC_F9,     KC_F10,   KC_F11,  KC_F12,   EE_CLR,   KC_MUTE,
        RGB_MOD,  KC_BT1,   KC_BT2,   KC_BT3,   KC_2G4,   KC_T,               KC_Y,     KC_U,     KC_I,     KC_O,      KC_P,     RGB_HUD, RGB_HUI,  KC_BSLS,  KC_INS,
        KC_CAPS,  KC_A,     TO(_MBL), KC_D,     KC_F,     KC_G,               KC_H,     KC_J,     KC_K,     KC_L,      RGB_SAD,  RGB_SAI, KC_ENT,             KC_HOME,
        KC_LSFT,  KC_Z,     RGB_TOG,  KC_C,     KC_V,     HS_BATQ,            NK_TOGG,  KC_M,     KC_COMM,  KC_DOT,    KC_SLSH,           KC_RSFT,  RGB_VAI,  KC_END,
        KC_FILP,  GU_TOGG,  KC_LALT,  KC_BATQ,                                KC_SPC,   TG(GAME), KC_NO,    KC_RCTL,                      RGB_SPD,  RGB_VAD,  RGB_SPI),

    [_MBL] = LAYOUT( /* Base */
        QK_GESC,  KC_1,     KC_2,     KC_3,     KC_4,     KC_5,               KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC, KC_MUTE,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,               KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS, KC_DEL,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,               KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,  KC_ENT,            KC_PGUP,
        KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,               KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,  KC_UP,   KC_PGDN,
        KC_LCTL,  KC_LALT,  KC_LGUI,  KC_SPC,                                 KC_SPC,   KC_RGUI,  MO(_MFL), KC_RCTL,                      KC_LEFT,  KC_DOWN, KC_RGHT),
    [_MFL] = LAYOUT( /* Base */
        KC_GRV,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,              KC_F6,    KC_F7,    KC_F8,    KC_F9,     KC_F10,   KC_F11,  KC_F12,   EE_CLR,   KC_MUTE,
        RGB_MOD,  KC_BT1,   KC_BT2,   KC_BT3,   KC_2G4,   KC_T,               KC_Y,     KC_U,     KC_I,     KC_O,      KC_P,     RGB_HUD, RGB_HUI,  KC_BSLS,  KC_INS,
        KC_CAPS,  TO(_BL),  KC_S,     KC_D,     KC_F,     KC_G,               KC_H,     KC_J,     KC_K,     KC_L,      RGB_SAD,  RGB_SAI, KC_ENT,             KC_HOME,
        KC_LSFT,  KC_Z,     RGB_TOG,  KC_C,     KC_V,     HS_BATQ,            NK_TOGG,  KC_M,     KC_COMM,  KC_DOT,    KC_SLSH,           KC_RSFT,  RGB_VAI,  KC_END,
        KC_FILP,  KC_LALT,  KC_LGUI,  KC_BATQ,                                KC_SPC,   KC_RGUI,  KC_RALT,  KC_RCTL,                      RGB_SPD,  RGB_VAD,  RGB_SPI),

    [GAME] = LAYOUT( /* Game */
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_ESC,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_B,               _______,
        _______,  _______,  _______,  _______,  _______,  _______,            _______,  KC_R,     KC_E,     KC_W,     KC_Q,     KC_TAB,   KC_B,     _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,            KC_G,     KC_F,     KC_A,     KC_S,     KC_D,     _______,  _______,            _______,
        _______,  _______,  _______,  _______,  _______,  _______,            _______,  KC_M,     KC_X,     KC_V,     KC_LCTL,            KC_LSFT,  _______,  _______,
        _______,  _______,  _______,  _______,                                _______,  _______,  MO(_FL),  KC_LCTL,                      _______,  _______,  _______),

};


#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [1] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [2] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [3] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
};
#endif
// clang-format on

bool is_keyboard_master(void) {
    setPinInput(SPLIT_HAND_PIN);
    return readPin(SPLIT_HAND_PIN);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        uprintf("Key pressed: %u\n", keycode);
    } else {
        uprintf("Key released: %u\n", keycode);
    }
    if (record->event.pressed) {
        switch (keycode) {
            case LEDTEST:
                led_index++;
                if (led_index >= 68) {
                    led_index = 0;
                }
                return false; // don’t fall through
        }
    }
    return true;
}

void set_game_leds(void) {

    for (uint8_t i = 0; i < LED_TOTAL; i++) {
        rgb_matrix_set_color(i, 0, 0, 0);
    }

    for (uint8_t i = 0; i < 7; i++) {
        HSV hsv = { 0, 255, 80 }; //rgb_matrix_get_val() };
        RGB rgb = hsv_to_rgb(hsv);
        rgb_matrix_set_color(GAME_LED_IDS[i], rgb.r, rgb.g, rgb.b);
    }
}

bool rgb_matrix_indicators_user(void) {

    // For checking led numbers
    // rgb_matrix_set_color(led_index, 255, 255, 255);

    if (IS_LAYER_ON(GAME)) {
        set_game_leds();
    }

    return true;  // <- important: return a bool
}

/*
 * NOT CALLED BECAUSE IT IS NOT A TRUE SPLIT KEYBOARD
 */
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {

    uint32_t mode = rgblight_get_mode();
    // assign colors if the matrix is on and the current mode
    // is SOLID COLORS => No animations running
    if(rgb_matrix_config.enable == 1 && mode == 1) { // && swap_colors) {

        // RGB rgb_color = hsv_to_rgb_keep_brightness(synced_data.keys_color[0], synced_data.keys_color[1], synced_data.keys_color[2]);

        // for (uint8_t i = led_min; i < led_max; i++) {
        //     // rgb_matrix_set_color(i, rgb_color.r, rgb_color.g, rgb_color.b);
        //     cycle_key_colors(i);
        // }

        // if (is_keyboard_left()) {
        //     cycle_underglow_colors(UG_LEFT_IDS, 8);
        // } else {
        //     cycle_underglow_colors(UG_RIGHT_IDS, 8);
        // }
    }

    if (IS_LAYER_ON(GAME)) {
        set_game_leds();
    }

    return true;
}
