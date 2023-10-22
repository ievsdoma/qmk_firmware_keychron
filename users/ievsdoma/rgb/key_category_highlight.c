#include "quantum.h"
#include "two_layers_tap_dance.h"
#include "colours.h"

typedef enum {
    KCC_ALFAS = 0,
    KCC_NUMERICS,
    KCC_MODS,
    KCC_FUNCTIONAL,
    KCC_NAV,
    KCC_SPECIAL,
    KCC_NONE
} sd_keycode_category;

static HSV keycode_category_colour_mapping[] = {
    [KCC_ALFAS] = {HSV_AZURE},
    [KCC_NUMERICS] = {HSV_GREEN},
    [KCC_MODS] = {HSV_BLUE},
    [KCC_FUNCTIONAL] = {HSV_HOT_PINK},
    [KCC_NAV] = {HSV_PURPLE},
    [KCC_SPECIAL] = {HSV_RED},
    [KCC_NONE] = {HSV_OFF}
};

static sd_keycode_category get_keycode_category(uint16_t keycode) {
    switch (keycode) {
        case KC_A:
        case KC_B:
        case KC_C:
        case KC_D:
        case KC_E:
        case KC_F:
        case KC_G:
        case KC_H:
        case KC_I:
        case KC_J:
        case KC_K:
        case KC_L:
        case KC_M:
        case KC_N:
        case KC_O:
        case KC_P:
        case KC_Q:
        case KC_R:
        case KC_S:
        case KC_T:
        case KC_U:
        case KC_V:
        case KC_W:
        case KC_X:
        case KC_Y:
        case KC_Z:
        case KC_LEFT_BRACKET:
        case KC_RIGHT_BRACKET:
        case KC_SEMICOLON:
        case KC_QUOTE:
        case KC_COMMA:
        case KC_DOT:
        case KC_SLASH:
            return KCC_ALFAS;
        case KC_1:
        case KC_2:
        case KC_3:
        case KC_4:
        case KC_5:
        case KC_6:
        case KC_7:
        case KC_8:
        case KC_9:
        case KC_0:
        case KC_MINUS:
        case KC_EQUAL:
            return KCC_NUMERICS;
        case KC_GRAVE:
        case KC_TAB:
        case KC_CAPS_LOCK:
        case CW_TOGG:
        case KC_LEFT_SHIFT:
        case KC_LEFT_CTRL:
        case KC_LWIN:
        case KC_LEFT_ALT:
        case KC_SPACE:
        case KC_RIGHT_ALT:
        case KC_RIGHT_CTRL:
        case KC_RIGHT_SHIFT:
        case KC_BACKSLASH:
        case KC_BACKSPACE:
        case KC_INSERT:
        // One momentary code per layer. More can be added if necessary
        case QK_MOMENTARY:
        case QK_MOMENTARY + 1:
        case QK_MOMENTARY + 2:
        case QK_MOMENTARY + 3:
        case QK_MOMENTARY + 4:
        // Only the first tap dance code. More can be added if necessary
        case QK_TAP_DANCE:
            return KCC_MODS;
        case KC_F1:
        case KC_F2:
        case KC_F3:
        case KC_F4:
        case KC_F5:
        case KC_F6:
        case KC_F7:
        case KC_F8:
        case KC_F9:
        case KC_F10:
        case KC_F11:
        case KC_F12:
            return KCC_FUNCTIONAL;
        case KC_PGDN:
        case KC_PGUP:
        case KC_HOME:
        case KC_END:
        case KC_DEL:
        case KC_PRINT_SCREEN:
        case KC_LEFT:
        case KC_UP:
        case KC_RIGHT:
        case KC_DOWN:
            return KCC_NAV;
        case KC_ESCAPE:
        case KC_ENTER:
            return KCC_SPECIAL;
        default:
            return KCC_NONE;
    }
}

static HSV get_keycode_colour(uint16_t keycode) {

    sd_keycode_category keycode_category = get_keycode_category(keycode);

    return keycode_category_colour_mapping[keycode_category];
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

                uint16_t keycode = keymap_key_to_keycode(layer, (keypos_t){col,row});

                RGB rgb = hsv_to_rgb(get_keycode_colour(keycode));

                rgb_matrix_set_color(index, rgb.r, rgb.g, rgb.b);
            }
        }
    }

    return rgb_matrix_check_finished_leds(led_max);
}
