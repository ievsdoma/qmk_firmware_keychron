#include "quantum.h"
#include "two_layers_tap_dance.h"
#include "key_category_highlight.h"

static bool DEFAULT_LAYER_KEY_CATEGORY_HIGHLIGHT(effect_params_t* params) {

    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    // In Keychron Q11 default layer is set as 1 << (<MODE_TOGGLE_AT_MAC> ? 0 : 2)
    uint8_t layer = default_layer_state > 1 ? WIN_BASE : MAC_BASE;

    dprintf("DEFAULT_LAYER_KEY_CATEGORY_HIGHLIGHT: Layer %d\n", layer);

    key_category_highlight(layer, led_min, led_max);

    return rgb_matrix_check_finished_leds(led_max);
}
