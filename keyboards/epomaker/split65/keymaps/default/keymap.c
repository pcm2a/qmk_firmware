// Copyright 2025 EPOMAKER (@Epomaker)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "rgb_record/rgb_record.h"

enum layers {
    _BL = 0,
    _FL,
    _MBL,
    _MFL,
};

#define ______ HS_BLACK

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_BL] = LAYOUT( /* Base */
        KC_ESC,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,               KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC, KC_MUTE,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,               KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS, KC_DEL,   
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,               KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,  KC_ENT,            KC_PGUP,
        KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,               KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,  KC_UP,   KC_PGDN,
        KC_LCTL,  KC_LCMD,  KC_LALT,  KC_SPC,                                 KC_SPC,   KC_RALT,  MO(_FL),  KC_RCTL,                      KC_LEFT,  KC_DOWN, KC_RGHT),

    [_FL] = LAYOUT( /* Base */
        KC_GRV,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,              KC_F6,    KC_F7,    KC_F8,    KC_F9,     KC_F10,   KC_F11,  KC_F12,   EE_CLR,   _______,
        RGB_MOD,  KC_BT1,   KC_BT2,   KC_BT3,   KC_2G4,   _______,            _______,  _______,  _______, _______,    _______,  RGB_HUD, RGB_HUI,  _______,  KC_INS,  
        _______,  KC_A,     TO(_MBL), _______,  _______,  _______,            _______,  _______,  _______, _______,    RGB_SAD,  RGB_SAI, _______,            KC_HOME, 
        _______,  _______,  RGB_TOG,  _______,  _______,  _______,            NK_TOGG,  _______,  _______, _______,    _______,           _______,  RGB_VAI,  KC_END,
        KC_FILP,  GU_TOGG,  _______,  KC_BATQ,                                KC_BATQ,  _______,  _______, _______,                       RGB_SPD,  RGB_VAD,  RGB_SPI),

    [_MBL] = LAYOUT( /* Base */
        KC_ESC,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,               KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC, KC_MUTE,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,               KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS, KC_DEL,   
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,               KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,  KC_ENT,            KC_PGUP,
        KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,               KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,  KC_UP,   KC_PGDN,
        KC_LCTL,  KC_LALT,  KC_LGUI,  KC_SPC,                                 KC_SPC,   KC_RGUI,  MO(_MFL), KC_RCTL,                      KC_LEFT,  KC_DOWN, KC_RGHT),
    [_MFL] = LAYOUT( /* Base */
        KC_GRV,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,              KC_F6,    KC_F7,    KC_F8,    KC_F9,     KC_F10,   KC_F11,  KC_F12,   EE_CLR,   _______,
        RGB_MOD,  KC_BT1,   KC_BT2,   KC_BT3,   KC_2G4,   _______,            _______,  _______,  _______, _______,    _______,  RGB_HUD, RGB_HUI,  _______,  KC_INS,  
        _______,  TO(_BL),  KC_S,     _______,  _______,  _______,            _______,  _______,  _______, _______,    RGB_SAD,  RGB_SAI, _______,            KC_HOME, 
        _______,  _______,  RGB_TOG,  _______,  _______,  _______,            NK_TOGG,  _______,  _______, _______,    _______,           _______,  RGB_VAI,  KC_END,
        KC_FILP,  _______,  _______,  KC_BATQ,                                KC_BATQ,  _______,  _______, _______,                       RGB_SPD,  RGB_VAD,  RGB_SPI),

};


#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [1] = {ENCODER_CCW_CW(_______, _______)},
    [2] = {ENCODER_CCW_CW(_______, _______)},
    [3] = {ENCODER_CCW_CW(_______, _______)},
};
#endif
// clang-format on

bool is_keyboard_master(void) {
    setPinInput(SPLIT_HAND_PIN);
    return readPin(SPLIT_HAND_PIN);
}

