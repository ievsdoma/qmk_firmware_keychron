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
#include "features/custom_keycodes.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(/* Base */
                 KC_BTN4,       KC_BTN1, KC_BTN3, KC_BTN2,      KC_BTN5,        MO(2),      MO(1),      _______),
    [1] = LAYOUT(/* Base */
                 C(S(KC_TAB)),  C(KC_C), _______, C(KC_V),      C(KC_TAB),      MO(3),      _______,    _______),
    [2] = LAYOUT(/* Base */
                 KC_ESC,        KC_HOME, _______, KC_END,       KC_WWW_REFRESH, _______,    MO(3),      DPI_CONFIG),
    [3] = LAYOUT(/* Base */
                 _______,       C(KC_W), _______, C(S(KC_T)),   _______,        _______,    _______,    QK_BOOT),
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { ENCODER_CCW_CW(KC_MS_WH_DOWN, KC_MS_WH_UP) },
    [1] = { ENCODER_CCW_CW(KC_MS_WH_RIGHT, KC_MS_WH_LEFT) },
    [2] = { ENCODER_CCW_CW(FAST_SCROLL_DOWN, FAST_SCROLL_UP) },
    [3] = { ENCODER_CCW_CW(KC_MS_WH_DOWN, KC_MS_WH_UP) },
};
#endif // ENCODER_MAP_ENABLE

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_custom(keycode, record)) {
        return false;
    }
    return true;
}
