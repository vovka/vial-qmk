/**
 * Reactive keypress effect - keys light up when pressed
 * Underglow stays off, only per-key LEDs react to keypresses
 */

#include QMK_KEYBOARD_H
#include "dynamic_keymap.h"
#include "vial.h"

// LED layout constants
#define LEDS_PER_HALF 43
#define PERKEY_LEFT_START 0
#define PERKEY_LEFT_END 19      // 19 per-key LEDs on left (indices 0-18)
#define PERKEY_RIGHT_START 43
#define PERKEY_RIGHT_END 62     // 19 per-key LEDs on right (indices 43-61)

// Reactive effect settings
#define FADE_DURATION 300       // ms to fade out after key release

// Track key press state and timing for each LED
static uint32_t key_press_time[LEDS_PER_HALF * 2];  // timestamp when key was released (0 = currently held)
static bool     key_held[LEDS_PER_HALF * 2];        // true if key is currently held

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

// Find LED index for a given matrix position
// Returns 255 if no LED exists for this position
static uint8_t get_led_for_matrix(uint8_t row, uint8_t col) {
    uint8_t led_index = 255;
    // Use QMK's built-in function to find LED by matrix position
    // g_led_config.matrix_co[row][col] contains the LED index or NO_LED
    if (row < MATRIX_ROWS && col < MATRIX_COLS) {
        led_index = g_led_config.matrix_co[row][col];
    }
    return led_index;
}

// Track key presses for reactive effect
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    uint8_t led = get_led_for_matrix(record->event.key.row, record->event.key.col);

    if (led != NO_LED && led < sizeof(key_held)) {
        if (record->event.pressed) {
            // Key pressed - mark as held
            key_held[led] = true;
            key_press_time[led] = 0;  // 0 means currently held
        } else {
            // Key released - start fade timer
            key_held[led] = false;
            key_press_time[led] = timer_read32();
        }
    }

    return true;  // Continue processing
}

// RGB Matrix indicators - simple reactive keypress effect
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    for (uint8_t i = led_min; i < led_max; i++) {
        // Check if this is a per-key LED (not underglow)
        bool is_perkey = (i < PERKEY_LEFT_END) ||
                         (i >= PERKEY_RIGHT_START && i < PERKEY_RIGHT_END);

        if (!is_perkey) {
            // Underglow - turn off
            rgb_matrix_set_color(i, 0, 0, 0);
            continue;
        }

        // Per-key LED - check if held or fading
        if (key_held[i]) {
            // Key is held - full brightness white
            rgb_matrix_set_color(i, 255, 255, 255);
        } else if (key_press_time[i] > 0) {
            // Key was released - fade out
            uint32_t elapsed = timer_elapsed32(key_press_time[i]);

            if (elapsed < FADE_DURATION) {
                // Calculate fade (255 -> 0 over FADE_DURATION)
                uint8_t brightness = 255 - (255 * elapsed / FADE_DURATION);
                rgb_matrix_set_color(i, brightness, brightness, brightness);
            } else {
                // Fade complete - turn off and reset timer
                rgb_matrix_set_color(i, 0, 0, 0);
                key_press_time[i] = 0;
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
