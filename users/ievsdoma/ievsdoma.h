#pragma once

enum layers{
    MAC_BASE,
    MAC_FN,
    WIN_BASE,
    WIN_FN,
    SYSTEM_CMD,
};

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

const uint8_t* g_caps_indicators;
const uint8_t g_caps_indicators_count;

#define RGB_MATRIX_CAPS_INDICATORS_SET_COLOR(r, g, b) \
    for (uint8_t led = 0; led < g_caps_indicators_count; led++) { \
        RGB_MATRIX_INDICATOR_SET_COLOR(g_caps_indicators[led], r, g, b); \
    }
