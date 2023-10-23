#include QMK_KEYBOARD_H
#include "indicators.h"
#include "key_category_highlight.h"

#define RGB_MATRIX_CAPS_INDICATORS_SET_COLOR(r, g, b) \
    for (uint8_t led = 0; led < g_caps_indicators_count; led++) { \
        RGB_MATRIX_INDICATOR_SET_COLOR(g_caps_indicators[led], r, g, b); \
    }

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    bool continue_led_processing = true;

    uint8_t layer = get_highest_layer(layer_state);
    if (layer > 0) {
        key_category_highlight(layer, led_min, led_max);
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

bool rgb_matrix_was_disabled = false;
HSV last_colour_hsv;

void ensure_rgb_matrix_enabled(bool desired_state) {
    if (desired_state) {
        if (!rgb_matrix_is_enabled()) {
            rgb_matrix_was_disabled = true;
            last_colour_hsv = rgb_matrix_get_hsv();
            rgb_matrix_enable_noeeprom();
            rgb_matrix_sethsv_noeeprom(HSV_OFF);
        }
    } else {
        if (rgb_matrix_was_disabled) {
            rgb_matrix_was_disabled = false;
            rgb_matrix_sethsv_noeeprom(last_colour_hsv.h, last_colour_hsv.s, last_colour_hsv.v);
            rgb_matrix_disable_noeeprom();
        }
    }
}

void caps_word_set_user(bool active) {
    ensure_rgb_matrix_enabled(active);
}

layer_state_t layer_state_set_user(layer_state_t state) {
    ensure_rgb_matrix_enabled(get_highest_layer(state) > 0);

    return state;
}
