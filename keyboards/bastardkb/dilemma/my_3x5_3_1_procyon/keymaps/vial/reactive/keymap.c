/**
 * Reactive keypress effect - keys light up when pressed
 * Underglow stays off, only per-key LEDs react to keypresses
 */

#include QMK_KEYBOARD_H
#include "dynamic_keymap.h"
#include "vial.h"

// LED layout constants
#define LEDS_PER_HALF 43
#define PERKEY_LEFT_END 19      // 19 per-key LEDs on left (indices 0-18)
#define PERKEY_RIGHT_START 43
#define PERKEY_RIGHT_END 62     // 19 per-key LEDs on right (indices 43-61)
#define TOTAL_LEDS (LEDS_PER_HALF * 2)

// Reactive effect settings
#define FADE_DURATION_MIN 60    // ms, fastest fade when speed is max
#define FADE_DURATION_SCALE 2   // ms per speed step (0..255)

// Track release timing for fade effect
static uint32_t key_release_time[TOTAL_LEDS];  // timestamp when key was released (0 = not fading)
static bool     key_was_pressed[TOTAL_LEDS];   // track previous state to detect release

enum dilemma_keymap_layers {
    LAYER_BASE = 0,
    LAYER_NAV,
    LAYER_SYM,
    LAYER_NUM,
};

#define NAV QK_TRI_LAYER_LOWER
#define SYM QK_TRI_LAYER_UPPER

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT_split_3x5_3_and_knob(
  // ╭─────────────────────────────────────────────╮ ╭─────────────────────────────────────────────╮
          KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,       KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
  // ├─────────────────────────────────────────────┤ ├─────────────────────────────────────────────┤
          KC_A,    KC_S,    KC_D,    KC_F,    KC_G,       KC_H,    KC_J,    KC_K,    KC_L, KC_QUOT,
  // ├─────────────────────────────────────────────┤ ├─────────────────────────────────────────────┤
          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,       KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,
  // ╰─────────────────────────────────────────────┤ ├─────────────────────────────────────────────╯
                         KC_LCTL,     NAV, KC_LGUI, KC_MPLY,  KC_MUTE,  KC_SPC,     SYM, KC_LALT
  //                   ╰───────────────────────────╯ ╰──────────────────────────╯
  ),

  [LAYER_NAV] = LAYOUT_split_3x5_3_and_knob(
  // ╭─────────────────────────────────────────────╮ ╭─────────────────────────────────────────────╮
        KC_TAB, XXXXXXX, XXXXXXX, XXXXXXX, KC_VOLU,    XXXXXXX, KC_HOME,   KC_UP,  KC_END,  KC_DEL,
  // ├─────────────────────────────────────────────┤ ├─────────────────────────────────────────────┤
       KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI, KC_VOLD,    XXXXXXX, KC_LEFT, KC_DOWN, KC_RGHT, KC_BSPC,
  // ├─────────────────────────────────────────────┤ ├─────────────────────────────────────────────┤
       QK_BOOT,  EE_CLR, KC_MPRV, KC_MNXT, KC_MPLY,    XXXXXXX, KC_PGDN, KC_PGUP, XXXXXXX,  KC_ENT,
  // ╰─────────────────────────────────────────────┤ ├─────────────────────────────────────────────╯
                         XXXXXXX, _______, KC_LSFT, _______,  _______,  KC_SPC, _______,  KC_ESC
  //                   ╰───────────────────────────╯ ╰──────────────────────────╯
  ),

  [LAYER_SYM] = LAYOUT_split_3x5_3_and_knob(
  // ╭─────────────────────────────────────────────╮ ╭─────────────────────────────────────────────╮
        KC_ESC, KC_LBRC, KC_LCBR, KC_LPRN, KC_TILD,    KC_CIRC, KC_RPRN, KC_RCBR, KC_RBRC,  KC_GRV,
  // ├─────────────────────────────────────────────┤ ├─────────────────────────────────────────────┤
       KC_MINS, KC_ASTR,  KC_EQL, KC_UNDS,  KC_DLR,    KC_HASH, KC_RGUI, KC_RALT, KC_RCTL, KC_RSFT,
  // ├─────────────────────────────────────────────┤ ├─────────────────────────────────────────────┤
       KC_PLUS, KC_PIPE,   KC_AT, KC_SLSH, KC_PERC,    KC_SCLN, KC_BSLS, KC_AMPR, KC_QUES, KC_EXLM,
  // ╰─────────────────────────────────────────────┤ ├─────────────────────────────────────────────╯
                          KC_ESC, _______, KC_LSFT, _______,  _______,  KC_SPC, _______, XXXXXXX
  //                   ╰───────────────────────────╯ ╰──────────────────────────╯
  ),

  [LAYER_NUM] = LAYOUT_split_3x5_3_and_knob(
  // ╭─────────────────────────────────────────────╮ ╭─────────────────────────────────────────────╮
          KC_1,    KC_2,    KC_3,    KC_4,    KC_5,       KC_6,    KC_7,    KC_8,    KC_9,   KC_0,
  // ├─────────────────────────────────────────────┤ ├─────────────────────────────────────────────┤
       KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI,  KC_F11,     KC_F12, KC_RGUI, KC_RALT, KC_RCTL, KC_RSFT,
  // ├─────────────────────────────────────────────┤ ├─────────────────────────────────────────────┤
         KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,      KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,
  // ╰─────────────────────────────────────────────┤ ├─────────────────────────────────────────────╯
                         XXXXXXX, _______, KC_LSFT, _______,  _______,  KC_SPC, _______, XXXXXXX
  //                   ╰───────────────────────────╯ ╰──────────────────────────╯
  ),
};
// clang-format on

// Encoder map required when ENCODER_MAP_ENABLE = yes
#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [LAYER_BASE] = { ENCODER_CCW_CW(KC_UP, KC_DOWN), ENCODER_CCW_CW(KC_LEFT, KC_RIGHT) },
    [LAYER_NAV]  = { ENCODER_CCW_CW(RGB_HUD, RGB_HUI), ENCODER_CCW_CW(RGB_SAD, RGB_SAI) },
    [LAYER_SYM]  = { ENCODER_CCW_CW(RGB_VAD, RGB_VAI), ENCODER_CCW_CW(RGB_SPD, RGB_SPI) },
    [LAYER_NUM]  = { ENCODER_CCW_CW(RGB_RMOD, RGB_MOD), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
};
#endif

// Check if a key at matrix position is currently pressed
// Uses the synced matrix state which works on both halves
static bool matrix_key_pressed(uint8_t row, uint8_t col) {
    if (row >= MATRIX_ROWS || col >= MATRIX_COLS) {
        return false;
    }
    return matrix_is_on(row, col);
}

static uint16_t reactive_fade_duration_ms(void) {
    // Faster RGB matrix speed should mean shorter fade time.
    uint8_t speed = rgb_matrix_get_speed();
    return FADE_DURATION_MIN + (uint16_t)(255 - speed) * FADE_DURATION_SCALE;
}

static rgb_t reactive_rgb_with_fade(hsv_t base_hsv, uint8_t fade) {
    if (fade < 255) {
        base_hsv.v = (uint16_t)base_hsv.v * fade / 255;
    }
    return hsv_to_rgb(base_hsv);
}

// RGB Matrix indicators - simple reactive keypress effect
// Uses matrix state directly (synced between halves) instead of process_record_user
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    hsv_t   base_hsv      = rgb_matrix_get_hsv();
    rgb_t   base_rgb      = hsv_to_rgb(base_hsv);
    uint16_t fade_duration = reactive_fade_duration_ms();

    for (uint8_t i = led_min; i < led_max; i++) {
        // Check if this is a per-key LED (not underglow)
        bool is_perkey = (i < PERKEY_LEFT_END) ||
                         (i >= PERKEY_RIGHT_START && i < PERKEY_RIGHT_END);

        if (!is_perkey) {
            // Underglow - turn off
            rgb_matrix_set_color(i, 0, 0, 0);
            continue;
        }

        // Find which matrix position this LED corresponds to
        // by searching through g_led_config.matrix_co
        bool key_is_pressed = false;
        for (uint8_t row = 0; row < MATRIX_ROWS && !key_is_pressed; row++) {
            for (uint8_t col = 0; col < MATRIX_COLS && !key_is_pressed; col++) {
                if (g_led_config.matrix_co[row][col] == i) {
                    key_is_pressed = matrix_key_pressed(row, col);
                }
            }
        }

        // Track state changes for fade effect
        if (key_is_pressed) {
            // Key is held - full brightness current HSV
            rgb_matrix_set_color(i, base_rgb.r, base_rgb.g, base_rgb.b);
            key_was_pressed[i] = true;
            key_release_time[i] = 0;  // Reset fade timer
        } else if (key_was_pressed[i] && key_release_time[i] == 0) {
            // Key just released - start fade
            key_release_time[i] = timer_read32();
            if (key_release_time[i] == 0) key_release_time[i] = 1;  // Avoid 0
            key_was_pressed[i] = false;
            rgb_matrix_set_color(i, base_rgb.r, base_rgb.g, base_rgb.b);
        } else if (key_release_time[i] > 0) {
            // Key released - fade out
            uint32_t elapsed = timer_elapsed32(key_release_time[i]);

            if (elapsed < fade_duration) {
                // Calculate fade (255 -> 0 over fade_duration)
                uint8_t brightness = 255 - (255 * elapsed / fade_duration);
                rgb_t   rgb        = reactive_rgb_with_fade(base_hsv, brightness);
                rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
            } else {
                // Fade complete - turn off and reset timer
                rgb_matrix_set_color(i, 0, 0, 0);
                key_release_time[i] = 0;
            }
        } else {
            // Key not active - off
            rgb_matrix_set_color(i, 0, 0, 0);
        }
    }

    return false;  // Let RGB matrix continue processing
}

void eeconfig_init_user(void) {
#ifdef VIAL_COMBO_ENABLE
    vial_combo_entry_t combo = { .input = { KC_Q, KC_T, KC_B, COMBO_END }, .output = QK_BOOT };
    dynamic_keymap_set_combo(0, &combo);

    combo = (vial_combo_entry_t){ .input = { KC_N, KC_Y, KC_P, COMBO_END }, .output = QK_BOOT };
    dynamic_keymap_set_combo(1, &combo);
    vial_init();
#endif
}

static bool combo_slot_is_empty(uint8_t index) {
#ifdef VIAL_COMBO_ENABLE
    vial_combo_entry_t combo = { 0 };
    if (dynamic_keymap_get_combo(index, &combo) != 0) {
        return false;
    }
    if (combo.output != 0) {
        return false;
    }
    for (size_t i = 0; i < ARRAY_SIZE(combo.input); ++i) {
        if (combo.input[i] != 0) {
            return false;
        }
    }
    return true;
#else
    (void)index;
    return false;
#endif
}

void keyboard_post_init_user(void) {
#ifdef VIAL_COMBO_ENABLE
    bool changed = false;

    if (combo_slot_is_empty(0)) {
        vial_combo_entry_t combo = { .input = { KC_Q, KC_T, KC_B, COMBO_END }, .output = QK_BOOT };
        dynamic_keymap_set_combo(0, &combo);
        changed = true;
    }
    if (combo_slot_is_empty(1)) {
        vial_combo_entry_t combo = { .input = { KC_N, KC_Y, KC_P, COMBO_END }, .output = QK_BOOT };
        dynamic_keymap_set_combo(1, &combo);
        changed = true;
    }
    if (changed) {
        vial_init();
    }
#endif
}
