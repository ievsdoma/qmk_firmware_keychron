#include "custom_keycodes.h"
#include <math.h>

#define MAX_SCROLL_SPEED 10

#define FAST_SCROLL(scroll_code) \
    for(uint8_t i = 0; i < scroll_multiplier; i++) { \
        tap_code16(scroll_code); \
    }

uint8_t scroll_multiplier = 3;
bool scroll_is_vertical = true;

void print_fast_scroll_speed(void) {
#    ifdef SEND_STRING_ENABLE
    const char *scroll_multiplier_str = get_u8_str(scroll_multiplier, ' ');
    // Skip padding spaces
    while (*scroll_multiplier_str == ' ') {
        scroll_multiplier_str++;
    }
    send_string(scroll_multiplier_str);
#    endif
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    uint16_t scroll_code;

    if (!record->event.pressed) {
        return true;
    }

    switch (keycode) {
        case FAST_SCROLL_UP:
        case FS_UP:
            scroll_code = scroll_is_vertical ? KC_MS_WH_UP : KC_MS_WH_LEFT;
            FAST_SCROLL(scroll_code);
            return false; // Skip all further processing of this key
        case FAST_SCROLL_DOWN:
        case FS_DOWN:
            scroll_code = scroll_is_vertical ? KC_MS_WH_DOWN : KC_MS_WH_RIGHT;
            FAST_SCROLL(scroll_code);
            return false;
        case FAST_SCROLL_CHANGE_DIRECTION:
        case FS_CDIR:
            scroll_is_vertical = !scroll_is_vertical;
            return false;
        case FAST_SCROLL_SPEED_UP:
        case FS_SPI:
            scroll_multiplier = MIN(MAX_SCROLL_SPEED, scroll_multiplier + 1);
            return false;
        case FAST_SCROLL_SPEED_DOWN:
        case FS_SPD:
            scroll_multiplier = MAX(1, scroll_multiplier - 1);
            return false;
        case FAST_SCROLL_PRINT_SPEED:
        case FS_PRNS:
            print_fast_scroll_speed();
            return false;
        default:
            return true; // Process all other keycodes normally
    }
}
