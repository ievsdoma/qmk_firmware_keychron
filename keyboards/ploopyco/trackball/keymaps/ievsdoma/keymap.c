/* Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2019 Sunjun Kim
 * Copyright 2020 Ploopy Corporation
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

enum custom_keycodes {
    TAP_HOLD_DRAG_SCROLL = SAFE_RANGE
};

enum custom_layers {
    _BASE,   // Layer 0
    _COPYPASTE,  // Layer 1
    _NAVIGATE
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        MS_BTN1, TAP_HOLD_DRAG_SCROLL, LT(_NAVIGATE, KC_WWW_REFRESH), MS_BTN2, LT(_COPYPASTE, KC_ESC)
    ),
    [_COPYPASTE] = LAYOUT(
        C(KC_C), C(KC_W), C(KC_V), KC_WWW_REFRESH, _______
    ),
    [_NAVIGATE] = LAYOUT(
        _______, _______, _______, MS_BTN4, MS_BTN5
    ),
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_BASE] = { ENCODER_CCW_CW(_______, _______) },
    [_COPYPASTE] = { ENCODER_CCW_CW(MS_BTN4, MS_BTN5) },
    [_NAVIGATE] = { ENCODER_CCW_CW(_______, _______) },
};
#endif // ENCODER_MAP_ENABLE

// Track the state of the button
static uint16_t tap_hold_timer = 0;
static bool button_is_pressed = false;
static bool drag_scroll_engaged = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TAP_HOLD_DRAG_SCROLL:
            if (record->event.pressed) {
                // Button went DOWN
                tap_hold_timer = timer_read();
                button_is_pressed = true;
                drag_scroll_engaged = false;
            } else {
                // Button went UP
                button_is_pressed = false;

                if (drag_scroll_engaged) {
                    // It was a hold: Turn off drag scroll by sending a false release record
                    keyrecord_t drag_record = *record;
                    drag_record.event.pressed = false;
                    process_record_kb(DRAG_SCROLL, &drag_record);
                    drag_scroll_engaged = false;
                } else {
                    // It was a short press: Simulate a normal mouse click tap
                    register_code16(MS_BTN3); // Change this to your desired tap command
                    unregister_code16(MS_BTN3);
                }

                tap_hold_timer = 0; // Clear the timer safely
            }
            return false; // Handled

        default:
            return true; // Pass through all other keys
    }
}

void matrix_scan_user(void) {
    // ONLY check the timer if the physical button is currently being held down
    if (button_is_pressed && !drag_scroll_engaged) {
        if (timer_elapsed(tap_hold_timer) > TAPPING_TERM) {
            drag_scroll_engaged = true;

            // Forcefully inject the drag scroll press event
            keyrecord_t drag_record;
            drag_record.event.pressed = true;
            drag_record.event.time = timer_read();
            process_record_kb(DRAG_SCROLL, &drag_record);
        }
    }
}
