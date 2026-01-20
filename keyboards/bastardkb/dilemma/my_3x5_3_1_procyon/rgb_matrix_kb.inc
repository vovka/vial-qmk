// Custom RGB Matrix Effect: Reactive Per-Key Fade
// Keys light up on press and fade out based on g_last_hit_tracker, underglow stays off

#ifdef RGB_MATRIX_KEYREACTIVE_ENABLED

RGB_MATRIX_EFFECT(REACTIVE_PERKEY_FADE)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

// LED layout constants
#define PERKEY_LEFT_END 19
#define PERKEY_RIGHT_START 43
#define PERKEY_RIGHT_END 62

// Fade timing constants
#define FADE_DURATION_MIN 60
#define FADE_DURATION_SCALE 2

static uint16_t reactive_fade_duration_ms(void) {
    uint8_t speed = rgb_matrix_get_speed();
    return FADE_DURATION_MIN + (uint16_t)(255 - speed) * FADE_DURATION_SCALE;
}

static bool REACTIVE_PERKEY_FADE(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    hsv_t base_hsv = rgb_matrix_get_hsv();
    uint16_t fade_duration = reactive_fade_duration_ms();

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();

        // Check if this is a per-key LED (not underglow)
        bool is_perkey = (i < PERKEY_LEFT_END) ||
                         (i >= PERKEY_RIGHT_START && i < PERKEY_RIGHT_END);

        if (!is_perkey) {
            rgb_matrix_set_color(i, 0, 0, 0);  // Underglow off
            continue;
        }

        // Get last hit time for this LED from g_last_hit_tracker
        uint16_t time_since_hit = UINT16_MAX;
        for (uint8_t j = 0; j < g_last_hit_tracker.count; j++) {
            if (g_last_hit_tracker.index[j] == i) {
                time_since_hit = g_rgb_timer - g_last_hit_tracker.tick[j];
                break;
            }
        }

        // Calculate fade based on time since last hit
        if (time_since_hit < fade_duration) {
            // Fade from full brightness to off
            uint8_t brightness = 255 - (255 * time_since_hit / fade_duration);
            hsv_t fade_hsv = base_hsv;
            fade_hsv.v = (uint16_t)fade_hsv.v * brightness / 255;
            rgb_t rgb = hsv_to_rgb(fade_hsv);
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        } else {
            // Fully faded, turn off
            rgb_matrix_set_color(i, 0, 0, 0);
        }
    }

    return rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // RGB_MATRIX_KEYREACTIVE_ENABLED
