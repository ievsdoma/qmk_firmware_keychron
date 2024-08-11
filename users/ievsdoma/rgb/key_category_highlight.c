#include "quantum.h"
#include "keychron_common.h"
#include <lib/lib8tion/lib8tion.h>
#include "colours.h"
#include "key_category_highlight.h"

typedef enum {
    KCC_ALFAS = 0,
    KCC_NUMERICS,
    KCC_MATH,
    KCC_MODS,
    KCC_FUNCTIONAL,
    KCC_NAV,
    KCC_SPECIAL,
    KCC_QDEV,
    KCC_RGB_MODE,
    KCC_RGB_BRIGHTNESS,
    KCC_RGB_HUE,
    KCC_RGB_SATURATION,
    KCC_RGB_SPEED,
    KCC_SCREEN_BRIGHTNESS,
    KCC_MEDIA_CONTROLS,
    KCC_VOLUME_CONTROLS,
    KCC_OTHER,
    KCC_BLUETOOTH,
    KCC_WIRELESS,
    KCC_NONE
} sd_keycode_category;

static HSV keycode_category_colour_mapping[] = {
    [KCC_ALFAS] = {HSV_AZURE},
    [KCC_NUMERICS] = {HSV_GREEN},
    [KCC_MATH] = {HSV_PURPLE},
    [KCC_MODS] = {HSV_BLUE},
    [KCC_FUNCTIONAL] = {HSV_HOT_PINK},
    [KCC_NAV] = {HSV_PURPLE},
    [KCC_SPECIAL] = {HSV_RED},
    [KCC_QDEV] = {HSV_RED},
    [KCC_RGB_MODE] = {HSV_MAGENTA},
    [KCC_RGB_BRIGHTNESS] = {HSV_WHITE},
    [KCC_RGB_HUE] = {HSV_GOLD},
    [KCC_RGB_SATURATION] = {HSV_CYAN},
    [KCC_RGB_SPEED] = {HSV_RED},
    [KCC_SCREEN_BRIGHTNESS] = {HSV_YELLOW},
    [KCC_MEDIA_CONTROLS] = {HSV_BLUE},
    [KCC_VOLUME_CONTROLS] = {HSV_TURQUOISE},
    [KCC_OTHER] = {HSV_PINK},
    [KCC_BLUETOOTH] = {HSV_BLUE},
    [KCC_WIRELESS] = {HSV_YELLOW},
    [KCC_NONE] = {HSV_OFF}
};

static uint8_t get_numpad_category(uint16_t keycode) {
    return host_keyboard_led_state().num_lock ? KCC_NUMERICS : KCC_NAV;
}

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
            return KCC_NUMERICS;
        case KC_KP_1:
        case KC_KP_2:
        case KC_KP_3:
        case KC_KP_4:
        case KC_KP_5:
        case KC_KP_6:
        case KC_KP_7:
        case KC_KP_8:
        case KC_KP_9:
        case KC_KP_0:
            return get_numpad_category(keycode);
        case KC_MINUS:
        case KC_EQUAL:
        case KC_KP_MINUS:
        case KC_KP_PLUS:
        case KC_KP_SLASH:
        case KC_KP_EQUAL:
        case KC_KP_ASTERISK:
        case KC_KP_COMMA:
        case KC_KP_DOT:
            return KCC_MATH;
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
        case KC_KP_ENTER:
        case KC_NUM_LOCK:
            return KCC_SPECIAL;
        case DB_TOGG:
        case QK_RBT:
        case EE_CLR:
        case QK_BOOT:
            return KCC_QDEV;
        case KC_BRID:
        case KC_BRIU:
            return KCC_SCREEN_BRIGHTNESS;
        case KC_MPRV:
        case KC_MPLY:
        case KC_MNXT:
            return KCC_MEDIA_CONTROLS;
        case KC_MUTE:
        case KC_VOLD:
        case KC_VOLU:
            return KCC_VOLUME_CONTROLS;
        case RGB_MOD:
        case RGB_RMOD:
            return KCC_RGB_MODE;
        case RGB_VAD:
        case RGB_VAI:
            return KCC_RGB_BRIGHTNESS;
        case RGB_HUI:
        case RGB_HUD:
            return KCC_RGB_HUE;
        case RGB_SAI:
        case RGB_SAD:
            return KCC_RGB_SATURATION;
        case RGB_SPI:
        case RGB_SPD:
            return KCC_RGB_SPEED;
        case BT_HST1:
        case BT_HST2:
        case BT_HST3:
            return KCC_BLUETOOTH;
        case P2P4G:
            return KCC_WIRELESS;
        case KC_NO:
        case KC_TRANSPARENT:
            return KCC_NONE;
        default:
            if (QK_MOD_TAP <= keycode && keycode <= QK_MOD_TAP_MAX) {
                return get_keycode_category(keycode&0xFF);
            }
            return KCC_OTHER;
    }
}

static HSV get_keycode_colour(uint16_t keycode) {

    sd_keycode_category keycode_category = get_keycode_category(keycode);

    return keycode_category_colour_mapping[keycode_category];
}

static bool should_apply_colour_processing(uint8_t keycode_category) {
    switch (keycode_category) {
        case KCC_BLUETOOTH:
        case KCC_WIRELESS:
        case KCC_QDEV:
            return false;
        default:
            return true;
    }
}

void key_category_highlight(uint8_t layer, uint8_t led_min, uint8_t led_max, colour_processor colour_processor_func) {

    uint8_t time = scale16by8(g_rgb_timer, rgb_matrix_config.speed / 4);

    for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
        for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
            uint8_t index = g_led_config.matrix_co[row][col];

            // set only leds in the range
            if (index >= led_min && index < led_max && index != NO_LED) {

                uint16_t keycode = keymap_key_to_keycode(layer, (keypos_t){col,row});

                HSV category_colour = get_keycode_colour(keycode);

                if (category_colour.v == 0) {
                    continue;
                }

                HSV adjusted_category_colour = should_apply_colour_processing(get_keycode_category(keycode))
                                                    ? colour_processor_func(category_colour, index, time)
                                                    : category_colour;

                RGB rgb = hsv_to_rgb(adjusted_category_colour);

                rgb_matrix_set_color(index, rgb.r, rgb.g, rgb.b);
            }
        }
    }
}
