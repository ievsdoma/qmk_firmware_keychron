#include "quantum.h"
#include <timer.h>
#include <lib/lib8tion/lib8tion.h>
#include "indicators.h"
#include "key_category_highlight.h"

#define RGB_TOGGLE_CHECK_RATE 100

#define RGB_MATRIX_CAPS_INDICATORS_SET_COLOR(r, g, b) \
    for (uint8_t led = 0; led < g_caps_indicators_count; led++) { \
        RGB_MATRIX_INDICATOR_SET_COLOR(g_caps_indicators[led], r, g, b); \
    }

static uint8_t last_layer_number = 0;

static HSV apply_rgb_matrix_value_and_hue(HSV input, uint8_t i, uint8_t time) {
    HSV output;
    output.h = (input.h + rgb_matrix_get_hue()) % 256;
    output.s = input.s;
    output.v = scale8(input.v, rgb_matrix_get_val());
    return output;
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    bool continue_led_processing = true;

    uint8_t layer = get_highest_layer(layer_state);
    if (layer > 0) {
        key_category_highlight(layer, led_min, led_max, &apply_rgb_matrix_value_and_hue);
    } else if (!rgb_matrix_get_flags() && last_layer_number > 0) {
        rgb_matrix_set_color_all(RGB_OFF);

        continue_led_processing = false;
    }

    last_layer_number = layer;

    if(is_caps_word_on()){
        RGB_MATRIX_CAPS_INDICATORS_SET_COLOR(255, 0, 0);

        continue_led_processing = false;

    } else if (!rgb_matrix_get_flags()) {
        RGB_MATRIX_CAPS_INDICATORS_SET_COLOR(0, 0, 0);

        continue_led_processing = false;
    }

    return continue_led_processing;
}

static bool i_turned_on_led = false;

static void ensure_rgb_matrix_enabled_worker(void) {
    if (!rgb_matrix_is_enabled()) {
        dprintln("ensure_rgb_matrix_enabled: rgb off");
        if (!i_turned_on_led) {
            dprintln("ensure_rgb_matrix_enabled: I didn't turn on rgb");
            rgb_matrix_enable_noeeprom();
            rgb_matrix_set_flags_noeeprom(LED_FLAG_NONE);
            i_turned_on_led = true;
        } else {
            dprintln("ensure_rgb_matrix_enabled: I turned on rgb");
            rgb_matrix_enable();
            rgb_matrix_set_flags(LED_FLAG_ALL);
            i_turned_on_led = false;
        }
    }
}

static bool maintain_rgb_matrix_enabled = false;

static void ensure_rgb_matrix_enabled(bool desired_state) {
    if (desired_state) {
        dprintln("ensure_rgb_matrix_enabled: Starting worker");
        maintain_rgb_matrix_enabled = true;
    } else {
        dprintln("ensure_rgb_matrix_enabled: Stopping worker");
        maintain_rgb_matrix_enabled = false;

        if (i_turned_on_led) {
            dprintln("ensure_rgb_matrix_enabled: Restore after I turned on rgb");
            rgb_matrix_reload_from_eeprom();
            i_turned_on_led = false;
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

static uint32_t last_execution_time = 0;

// We are not using deferred exec as Keychron's implementation of Bluetooth
// resets RGB matrix state from EEPROM in housekeeping_task_kb if there are no indicators enabled
void housekeeping_task_user(void) {
    if (maintain_rgb_matrix_enabled && timer_elapsed32(last_execution_time) > RGB_TOGGLE_CHECK_RATE) {
        ensure_rgb_matrix_enabled_worker();
        last_execution_time = timer_read32();
    }
}
