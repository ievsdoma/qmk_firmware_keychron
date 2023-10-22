#include "quantum.h"
#include "two_layers_tap_dance.h"

typedef struct _keycode_range_colour {
    uint8_t from;
    uint8_t to;
    HSV colour;
} keycode_range_colour;

static keycode_range_colour keycode_colour_mappings[] = {
    {
        KC_A, KC_Z,
        {HSV_AZURE}
    },
    {
        KC_LEFT_BRACKET, KC_SLASH,
        {HSV_AZURE}
    },
    {
        KC_1, KC_0,
        {HSV_GREEN}
    },
    {
        KC_LEFT_CTRL, KC_RIGHT_GUI,
        {HSV_BLUE}
    },
    {
        KC_F1, KC_F12,
        {HSV_PINK}
    },
    {
        KC_ESC, KC_ESC,
        {HSV_RED}
    }
};

static HSV get_keycode_colour(uint8_t keycode) {

    uint8_t map_cnt = sizeof(keycode_colour_mappings)/sizeof(keycode_colour_mappings[0]);

    for (uint8_t map_idx = 0; map_idx < map_cnt; map_idx++) {
        keycode_range_colour map = keycode_colour_mappings[map_idx];
        if (map.from <= keycode && keycode <= map.to) {
            return map.colour;
        }
    }

    HSV black = {HSV_BLACK};

    return black;
}

static bool KEY_CATEGORY_HIGHLIGHT(effect_params_t* params) {

    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    uint8_t layer = get_highest_layer(layer_state);

    if (layer == MAC_BASE) {
        layer = log(default_layer_state)/log(2);
    }

    dprintf("KEY_CATEGORY_HIGHLIGHT: Layer %d\n", layer);

    for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
        for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
            uint8_t index = g_led_config.matrix_co[row][col];

            // set only leds in the range
            if (index >= led_min && index < led_max && index != NO_LED) {

                uint8_t keycode = keymap_key_to_keycode(layer, (keypos_t){col,row});

                RGB rgb = hsv_to_rgb(get_keycode_colour(keycode));

                rgb_matrix_set_color(index, rgb.r, rgb.g, rgb.b);
            }
        }
    }

    return rgb_matrix_check_finished_leds(led_max);
}
