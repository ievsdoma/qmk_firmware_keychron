#include QMK_KEYBOARD_H
#include "ievsdoma.h"

// Declare the functions to be used with your tap dance key(s)

// Function associated with all tap dances
td_state_t cur_dance(tap_dance_state_t *state);

// Functions associated with individual tap dances
void ql_finished(tap_dance_state_t *state, void *user_data);
void ql_reset(tap_dance_state_t *state, void *user_data);

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
        RGB_MATRIX_CAPS_INDICATORS_SET_COLOR(255, 0, 0);

        continue_led_processing = false;

    } else if (!rgb_matrix_get_flags()) {
        RGB_MATRIX_CAPS_INDICATORS_SET_COLOR(0, 0, 0);

        continue_led_processing = false;
    }

    return continue_led_processing;
}
