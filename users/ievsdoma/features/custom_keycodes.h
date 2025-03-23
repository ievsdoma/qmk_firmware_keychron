#pragma once

#include "quantum.h"

enum custom_keycodes {
    FAST_SCROLL_UP = SAFE_RANGE,
    FS_UP,
    FAST_SCROLL_DOWN,
    FS_DOWN,
    FAST_SCROLL_LEFT,
    FS_LEFT,
    FAST_SCROLL_RIGHT,
    FS_RIGHT,
    FAST_SCROLL_CHANGE_DIRECTION,
    FS_CDIR,
    FAST_SCROLL_SPEED_UP,
    FS_SPI,
    FAST_SCROLL_SPEED_DOWN,
    FS_SPD,
    FAST_SCROLL_PRINT_SPEED,
    FS_PRNS
};

bool process_record_custom(uint16_t keycode, keyrecord_t *record);
