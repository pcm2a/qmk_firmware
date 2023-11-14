/*
Copyright 2017 Balz Guenat <balz.guenat@gmail.com>

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

#include QMK_KEYBOARD_H

uint8_t lastKeyPressed = 0;

#define NUM_LOCK_BITMASK 0b01
#define CAPS_LOCK_BITMASK 0b10

// World record for fastest index finger tapping is 1092 taps per minute, which
// is 55ms for a single tap.
// https://recordsetter.com/world-record/index-finger-taps-minute/46066
#define LED_CMD_TIMEOUT 1000

typedef enum {
    // You could theoretically define 0b00 and send it by having a macro send
    // the second tap after LED_CMD_TIMEOUT has elapsed.
    // CMD_EXTRA = 0b00,
    CMD_KVM = 0b01,
    CMD_QQQ   = 0b10,
    CMD_RESET = 0b11 // CMD_ prefix to avoid clash with QMK macro
} led_cmd_t;

// State
static bool   num_lock_state  = false;
static bool   caps_lock_state = false;
static bool   in_cmd_window   = false;

typedef struct {
    led_cmd_t led_cmd;
    uint8_t   num_lock_count;
    uint8_t   caps_lock_count;
} cmd_window_state_t;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* 0: plain Qwerty without layer switching
     *         ,---------------. ,---------------. ,---------------.
     *         |F13|F14|F15|F16| |F17|F18|F19|F20| |F21|F22|F23|F24|
     * ,---.   |---------------| |---------------| |---------------| ,-----------. ,---------------. ,-------.
     * |Esc|   |F1 |F2 |F3 |F4 | |F5 |F6 |F7 |F8 | |F9 |F10|F11|F12| |PrS|ScL|Pau| |VDn|VUp|Mut|Pwr| | Help  |
     * `---'   `---------------' `---------------' `---------------' `-----------' `---------------' `-------'
     * ,-----------------------------------------------------------. ,-----------. ,---------------. ,-------.
     * |  `|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|JPY|Bsp| |Ins|Hom|PgU| |NmL|  /|  *|  -| |Stp|Agn|
     * |-----------------------------------------------------------| |-----------| |---------------| |-------|
     * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|  \  | |Del|End|PgD| |  7|  8|  9|  +| |Mnu|Und|
     * |-----------------------------------------------------------| `-----------' |---------------| |-------|
     * |CapsL |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  :|  #|Retn|               |  4|  5|  6|KP,| |Sel|Cpy|
     * |-----------------------------------------------------------|     ,---.     |---------------| |-------|
     * |Shft|  <|  Z|  X|  C|  V|  B|  N|  M|  ,|  ,|  /| RO|Shift |     |Up |     |  1|  2|  3|KP=| |Exe|Pst|
     * |-----------------------------------------------------------| ,-----------. |---------------| |-------|
     * |Ctl|Gui|Alt|MHEN|HNJ| Space  |H/E|HENK|KANA|Alt|Gui|App|Ctl| |Lef|Dow|Rig| |  0    |  .|Ent| |Fnd|Cut|
     * `-----------------------------------------------------------' `-----------' `---------------' `-------'
     */
    [0] = LAYOUT_all(
                      KC_F13,  KC_F14,  KC_F15,  KC_F16, KC_F17, KC_F18, KC_F19,  KC_F20,  KC_F21,  KC_F22,  KC_F23,  KC_F24,
    KC_ESC,           KC_F1,   KC_F2,   KC_F3,   KC_F4,  KC_F5,  KC_F6,  KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,               KC_PSCR, KC_SCRL, KC_PAUS,    KC_VOLD, KC_VOLU, KC_MUTE, KC_PWR,     KC_HELP,
    KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,   KC_6,   KC_7,   KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_INT3, KC_BSPC,     KC_INS,  KC_HOME, KC_PGUP,    KC_NUM,  KC_PSLS, KC_PAST, KC_PMNS,    KC_STOP, KC_AGIN,
    KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,   KC_Y,   KC_U,   KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC,          KC_BSLS,     KC_DEL,  KC_END,  KC_PGDN,    KC_P7,   KC_P8,   KC_P9,   KC_PPLS,    KC_MENU, KC_UNDO,
    KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,   KC_H,   KC_J,   KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_NUHS, KC_ENT,                                    KC_P4,   KC_P5,   KC_P6,   KC_PCMM,    KC_SLCT, KC_COPY,
    KC_LSFT, KC_NUBS, KC_Z,    KC_X,    KC_C,    KC_V,   KC_B,   KC_N,   KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_INT1, KC_RSFT,              KC_UP,               KC_P1,   KC_P2,   KC_P3,   KC_PEQL,    KC_EXEC, KC_PSTE,
    KC_LCTL, KC_LGUI, KC_LALT, KC_INT5, KC_LNG2,         KC_SPC,         KC_LNG1, KC_INT4, KC_INT2, KC_RALT, KC_RGUI, KC_APP,  KC_RCTL,     KC_LEFT, KC_DOWN, KC_RGHT,    KC_P0,            KC_PDOT, KC_PENT,    KC_FIND, KC_CUT
    ),
    [1] = LAYOUT_all(
                      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,           _______, _______, _______,    _______, _______, _______, _______,    _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______, _______, _______,    _______, _______, _______, _______,    _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,  _______, _______, _______,    _______, _______, _______, _______,    _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______,                                _______, _______, _______, _______,    _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______,           _______,             _______, _______, _______, _______,    _______, _______,
    _______, _______, _______, _______, _______,          _______,          _______, _______, _______, _______, _______, _______, _______,  _______, _______, _______,    _______,          _______, _______,    _______, _______
    ),
};

uint32_t numlock_clock = 0;

bool oled_task_user(void) {

    // Host Keyboard Layer Status
    oled_write_P(PSTR("Layer: "), false);

    switch (get_highest_layer(layer_state)) {
        case 0:
            oled_write_P(PSTR("Default\n"), false);
            break;
        case 1:
            oled_write_P(PSTR("FN\n"), false);
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_ln_P(PSTR("Undefined"), false);
    }


    oled_write_P(PSTR("Last: "), false);

    //char value[21] = {0};
    //char reportStr[30];
    //itoa(lastKeyPressed, value, 10);
    //strcpy(reportStr, "Last: ");
    //strncat(reportStr, value, 21);
    //strcat(reportStr, "\n");
    //oled_write_P(value, false);

    oled_write_P(PSTR("Last: "), false);

    //char str[10];
    //sprintf(str, "%d", 57);
    //oled_write_P(str, false);

    oled_write(get_u8_str(lastKeyPressed, '0'), false);

    oled_write_P(PSTR("\n"), false);

    // Host Keyboard LED Status
    led_t led_state = host_keyboard_led_state();
    oled_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
    oled_write_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
    oled_write_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);

    oled_write(get_u16_str(numlock_clock, '0'), false);
    oled_write_P(PSTR("\n"), false);

    return false;
}


void keyboard_post_init_user(void) {
    num_lock_state  = host_keyboard_led_state().num_lock;
    caps_lock_state = host_keyboard_led_state().caps_lock;
}

uint32_t command_timeout(uint32_t trigger_time, void *cb_arg) {
    SEND_STRING("DEBUG 3\n");
    cmd_window_state_t *cmd_window_state = (cmd_window_state_t *)cb_arg;
    switch (cmd_window_state->led_cmd) {
        case CMD_KVM:
            SEND_STRING("DEBUG 4\n");
            tap_code(KC_RCTL);
            tap_code(KC_RCTL);
            break;
        case CMD_QQQ:
            break;
        case CMD_RESET:
            reset_keyboard();
            break;
        default:
            // Ignore unrecognised commands.
            break;
    }
    cmd_window_state->led_cmd         = 0;
    cmd_window_state->num_lock_count  = 0;
    cmd_window_state->caps_lock_count = 0;
    in_cmd_window                     = false;

    return 0; // Don't repeat
}

bool led_update_user(led_t led_state) {

    static cmd_window_state_t cmd_window_state = {
      .led_cmd = 0b00,
      .num_lock_count = 0,
      .caps_lock_count = 0
    };

    // Start timer to end command window if we are not already in the middle of
    // one.
    if (!in_cmd_window) {
        in_cmd_window = true;
        defer_exec(LED_CMD_TIMEOUT, command_timeout, &cmd_window_state);
    }

    // Set num lock and caps lock bits when each is toggled on and off within
    // the window.
    if (led_state.num_lock != num_lock_state) {
        cmd_window_state.num_lock_count++;
        SEND_STRING("DEBUG 1\n");

        if (cmd_window_state.num_lock_count == 2) {
            SEND_STRING("DEBUG 2\n");
            cmd_window_state.led_cmd |= NUM_LOCK_BITMASK;
            cmd_window_state.num_lock_count = 0;
        }
    }

    if (led_state.caps_lock != caps_lock_state) {
        cmd_window_state.caps_lock_count++;

        if (cmd_window_state.caps_lock_count == 2) {
            cmd_window_state.led_cmd |= CAPS_LOCK_BITMASK;
            cmd_window_state.caps_lock_count = 0;
        }
    }

    // Keep our copy of the LED states in sync with the host.
    num_lock_state  = led_state.num_lock;
    caps_lock_state = led_state.caps_lock;
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    if (!record->event.pressed) {
        lastKeyPressed = keycode;
    }
    return true;
};

