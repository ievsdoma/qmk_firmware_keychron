#pragma once

enum layers{
    MAC_BASE,
    MAC_FN,
    WIN_BASE,
    WIN_FN,
    SYSTEM_CMD,
};

enum {
    TWO_LAYR, // Our custom tap dance key; add any other tap dance keys to this enum
};

const uint8_t* g_caps_indicators;
const uint8_t g_caps_indicators_count;
