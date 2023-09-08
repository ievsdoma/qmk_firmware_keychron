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

enum layers{
    MAC_BASE,
    MAC_FN,
    WIN_BASE,
    WIN_FN,
    SYSTEM_CMD,
};

#define KC_TASK LGUI(KC_TAB)
#define KC_FLXP LGUI(KC_E)

// Define a type for as many tap dance states as you need
typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

enum {
    TWO_LAYR, // Our custom tap dance key; add any other tap dance keys to this enum
};

// Declare the functions to be used with your tap dance key(s)

// Function associated with all tap dances
td_state_t cur_dance(tap_dance_state_t *state);

// Functions associated with individual tap dances
void ql_finished(tap_dance_state_t *state, void *user_data);
void ql_reset(tap_dance_state_t *state, void *user_data);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [MAC_BASE] = LAYOUT_91_ansi(
        KC_MUTE,  KC_ESC,   KC_BRID,  KC_BRIU,     KC_MCTL,  KC_LPAD,    RGB_VAD,   RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,      KC_VOLU,  KC_INS,   KC_DEL,   KC_MUTE,
        _______,  KC_GRV,   KC_1,     KC_2,        KC_3,     KC_4,       KC_5,      KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,      KC_EQL,   KC_BSPC,            KC_PGUP,
        _______,  KC_TAB,   KC_Q,     KC_W,        KC_E,     KC_R,       KC_T,      KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,      KC_RBRC,  KC_BSLS,            KC_PGDN,
        _______,  KC_CAPS,  KC_A,     KC_S,        KC_D,     KC_F,       KC_G,      KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,                KC_ENT,             KC_HOME,
        _______,  KC_LSFT,            KC_Z,        KC_X,     KC_C,       KC_V,      KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,                KC_RSFT,  KC_UP,
        _______,  KC_LCTL,  KC_LOPT,  KC_LCMD,     MO(MAC_FN),           KC_SPC,                        KC_SPC,             KC_RCMD,  MO(MAC_FN),   KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [MAC_FN] = LAYOUT_91_ansi(
        RGB_TOG,  _______,  KC_F1,    KC_F2,       KC_F3,    KC_F4,      KC_F5,     KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,       KC_F12,   _______,  _______,  RGB_TOG,
        _______,  _______,  _______,  _______,     _______,  _______,    _______,   _______,  _______,  _______,  _______,  _______,  _______,      _______,  _______,            _______,
        _______,  RGB_TOG,  RGB_MOD,  RGB_VAI,     RGB_HUI,  RGB_SAI,    RGB_SPI,   _______,  _______,  _______,  _______,  _______,  _______,      _______,  _______,            _______,
        _______,  _______,  RGB_RMOD, RGB_VAD,     RGB_HUD,  RGB_SAD,    RGB_SPD,   _______,  _______,  _______,  _______,  _______,  _______,                _______,            _______,
        _______,  _______,            _______,     _______,  _______,    _______,   _______,  NK_TOGG,  _______,  _______,  _______,  _______,                _______,  _______,
        _______,  _______,  _______,  _______,     _______,              _______,                       _______,            _______,  _______,      _______,  _______,  _______,  _______),

    [WIN_BASE] = LAYOUT_91_ansi(
        KC_CALC,  KC_ESC,   KC_F1,    KC_F2,         KC_F3,    KC_F4,    KC_F5,     KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,       KC_F12,   KC_PSCR,  KC_DEL,   KC_MUTE,
        KC_HOME,  KC_GRV,   KC_1,     KC_2,          KC_3,     KC_4,     KC_5,      KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,      KC_EQL,   KC_BSPC,            KC_PGUP,
        KC_DEL,   KC_TAB,   KC_Q,     KC_W,          KC_E,     KC_R,     KC_T,      KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,      KC_RBRC,  KC_BSLS,            KC_END,
        KC_PGUP,  CW_TOGG,  KC_A,     KC_S,          KC_D,     KC_F,     KC_G,      KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,                KC_ENT,             KC_PGDN,
        KC_PGDN,  KC_LSFT,            KC_Z,          KC_X,     KC_C,     KC_V,      KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,                KC_RSFT,  KC_UP,
        KC_END,   KC_LCTL,  KC_LWIN,  TD(TWO_LAYR),  KC_LALT,            KC_SPC,                        KC_SPC,             KC_RALT,  TD(TWO_LAYR), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [WIN_FN] = LAYOUT_91_ansi(
        RGB_TOG,  _______,  KC_BRID,  KC_BRIU,     KC_TASK,  KC_FLXP,    RGB_VAD,   RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,      KC_VOLU,  _______,  _______,  RGB_TOG,
        _______,  _______,  _______,  _______,     _______,  _______,    _______,   _______,  _______,  _______,  _______,  _______,  _______,      _______,  _______,            _______,
        AS_RPT,   _______,  _______,  _______,     KC_KP_7,  KC_KP_8,    KC_KP_9,   RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,      _______,  _______,            _______,
        AS_UP,    KC_CAPS,  _______,  _______,     KC_KP_4,  KC_KP_5,    KC_KP_6,   RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,                _______,            _______,
        AS_DOWN,  AS_TOGG,            _______,     _______,  KC_KP_1,    KC_KP_2,   KC_KP_3,  NK_TOGG,  _______,  _______,  _______,  _______,                _______,  _______,
        _______,  _______,  _______,  _______,     _______,              KC_KP_0,                       _______,            _______,  _______,      _______,  _______,  _______,  _______),

    [SYSTEM_CMD] = LAYOUT_91_ansi(
        _______,  QK_BOOT,  _______,  _______,     _______,  _______,    _______,   _______,  _______,  _______,  _______,  _______,  _______,      _______,  _______,  QK_BOOT,  _______,
        _______,  _______,  _______,  _______,     _______,  _______,    _______,   _______,  _______,  _______,  _______,  _______,  _______,      _______,  _______,            _______,
        _______,  _______,  _______,  _______,     _______,  _______,    _______,   _______,  _______,  _______,  _______,  _______,  _______,      _______,  _______,            QK_RBT,
        _______,  _______,  _______,  _______,     _______,  _______,    _______,   _______,  _______,  _______,  _______,  _______,  _______,                _______,            EE_CLR,
        QK_RBT,   _______,            _______,     _______,  _______,    _______,   _______,  _______,  _______,  _______,  _______,  _______,                _______,  _______,
        EE_CLR,   _______,  _______,  _______,     _______,              _______,                       _______,            _______,  _______,      _______,  _______,  _______,  _______),
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [MAC_BASE]   = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [MAC_FN]     = { ENCODER_CCW_CW(RGB_VAD, RGB_VAI), ENCODER_CCW_CW(RGB_VAD, RGB_VAI) },
    [WIN_BASE]   = { ENCODER_CCW_CW(KC_PGUP, KC_PGDN), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [WIN_FN]     = { ENCODER_CCW_CW(RGB_VAD, RGB_VAI), ENCODER_CCW_CW(RGB_RMOD, RGB_MOD) },
    [SYSTEM_CMD] = { ENCODER_CCW_CW(RGB_HUD, RGB_HUI), ENCODER_CCW_CW(RGB_SPD, RGB_SPI) },
};
#endif // ENCODER_MAP_ENABLE

// Determine the current tap dance state
td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (!state->pressed) return TD_SINGLE_TAP;
        else return TD_SINGLE_HOLD;
    } else if (state->count == 2) {
        if (!state->pressed) return TD_DOUBLE_TAP;
        else return TD_DOUBLE_HOLD;
    }
    else return TD_UNKNOWN;
}

// Initialize tap structure associated with example tap dance key
static td_tap_t ql_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

// Functions that control what our tap dance key does
void ql_finished(tap_dance_state_t *state, void *user_data) {
    ql_tap_state.state = cur_dance(state);
    switch (ql_tap_state.state) {
        case TD_SINGLE_TAP:
            break;
        case TD_SINGLE_HOLD:
            layer_on(WIN_FN);
            break;
        case TD_DOUBLE_TAP:
            break;
        case TD_DOUBLE_HOLD:
            layer_on(SYSTEM_CMD);
            break;
        default:
            break;
    }
}

void ql_reset(tap_dance_state_t *state, void *user_data) {
    // If the key was held down and now is released then switch off the layer
    if (ql_tap_state.state == TD_SINGLE_HOLD) {
        layer_off(WIN_FN);
    }
    if (ql_tap_state.state == TD_DOUBLE_HOLD) {
        layer_off(SYSTEM_CMD);
    }
    ql_tap_state.state = TD_NONE;
}

// Associate our tap dance key with its functionality
tap_dance_action_t tap_dance_actions[] = {
    [TWO_LAYR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ql_finished, ql_reset)
};

// Set a long-ish tapping term for tap-dance keys
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QK_TAP_DANCE ... QK_TAP_DANCE_MAX:
            return 500;
        default:
            return TAPPING_TERM;
    }
}

uint8_t g_last_layer = 0;

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    bool continue_led_processing = true;

    if (get_highest_layer(layer_state) > 0) {
        uint8_t layer = get_highest_layer(layer_state);

        for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
            for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
                uint8_t index = g_led_config.matrix_co[row][col];

                // set only leds in the range
                if (index >= led_min && index < led_max && index != NO_LED && keymap_key_to_keycode(layer, (keypos_t){col,row}) > KC_TRNS) {
                    rgb_matrix_set_color(index, RGB_PURPLE);
                }
            }
        }

        g_last_layer = layer;

    } else if (!rgb_matrix_get_flags() && g_last_layer > 0) {
        for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
            for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
                uint8_t index = g_led_config.matrix_co[row][col];

                // set all leds at once and forget
                if (index != NO_LED && keymap_key_to_keycode(g_last_layer, (keypos_t){col,row}) > KC_TRNS) {
                    rgb_matrix_set_color(index, RGB_OFF);
                }
            }
        }

        g_last_layer = 0;
        continue_led_processing = false;
    }

    if(is_caps_word_on()){
        RGB_MATRIX_INDICATOR_SET_COLOR(16, 255, 0, 0);
        RGB_MATRIX_INDICATOR_SET_COLOR(22, 255, 0, 0);
        RGB_MATRIX_INDICATOR_SET_COLOR(23, 255, 0, 0);
        RGB_MATRIX_INDICATOR_SET_COLOR(24, 255, 0, 0);
        RGB_MATRIX_INDICATOR_SET_COLOR(30, 255, 0, 0);
    } else if (!rgb_matrix_get_flags()) {
        RGB_MATRIX_INDICATOR_SET_COLOR(16, 0, 0, 0);
        RGB_MATRIX_INDICATOR_SET_COLOR(22, 0, 0, 0);
        RGB_MATRIX_INDICATOR_SET_COLOR(23, 0, 0, 0);
        RGB_MATRIX_INDICATOR_SET_COLOR(24, 0, 0, 0);
        RGB_MATRIX_INDICATOR_SET_COLOR(30, 0, 0, 0);

        continue_led_processing = false;
    }

    return continue_led_processing;
}
