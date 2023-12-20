#pragma once

typedef HSV (*colour_processor)(HSV input, uint8_t i, uint8_t time);

void key_category_highlight(uint8_t layer, uint8_t led_min, uint8_t led_max, colour_processor colour_processor_func);
