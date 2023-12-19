#include "quantum.h"
#include <lib/lib8tion/lib8tion.h>
#include "features/two_layers_tap_dance.h"
#include "key_category_highlight.h"

static HSV BAND_SAT_math(HSV input, uint8_t i, uint8_t time) {
    HSV output;
    output.h = (input.h + rgb_matrix_get_hue()) % 256;
    output.s = scale8(input.s, rgb_matrix_get_sat());
    output.v = scale8(input.v, rgb_matrix_get_val());

    int16_t h = abs(scale8(g_led_config.point[i].x, 228) + 28 - time) * 8;
    output.h = (output.h + (h > 255 ? 0 : h)) % 256;
    output.s = h < 255 ? 255 : output.s;
    return output;
}

static bool DEFAULT_LAYER_KEY_CATEGORY_HIGHLIGHT_HUE_ROLL(effect_params_t* params) {

    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    // This is how Q11 disables RGB
    if (rgb_matrix_get_flags()) {

        // In Keychron Q11 default layer is set as 1 << (<MODE_TOGGLE_AT_MAC> ? 0 : 2)
        uint8_t layer = default_layer_state > 1 ? WIN_BASE : MAC_BASE;

        dprintf("DEFAULT_LAYER_KEY_CATEGORY_HIGHLIGHT: Layer %d\n", layer);

        key_category_highlight(layer, led_min, led_max, &BAND_SAT_math);
    }

    return rgb_matrix_check_finished_leds(led_max);
}
