// Sequential LED scanner for debugging - cycles through all LEDs
#pragma once

#include "print.h"

#define LED_CYCLE_TIME 500      // 500ms per LED
#define LEDS_PER_HALF 43        // 43 LEDs per half (indices 0-42)
#define PERKEY_COUNT 19         // 19 per-key LEDs (indices 0-18)
#define SPLASH_DURATION 3000    // 3 second red splash

// Debug: track last printed LED to avoid spamming console
static uint8_t last_printed_led = 255;

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    // Total cycle: 43 LEDs * 500ms * 2 halves + 3s splash = 46s total
    uint32_t scan_time_per_half = LEDS_PER_HALF * LED_CYCLE_TIME;  // 21500ms
    uint32_t total_scan_time = scan_time_per_half * 2;              // 43000ms
    uint32_t total_cycle = total_scan_time + SPLASH_DURATION;       // 46000ms

    uint32_t cycle_pos = g_rgb_timer % total_cycle;

    bool is_left_half = (led_min < LEDS_PER_HALF);

    // Turn off all LEDs in this batch first
    for (uint8_t i = led_min; i < led_max; i++) {
        rgb_matrix_set_color(i, 0, 0, 0);
    }

    if (cycle_pos >= total_scan_time) {
        // SPLASH PHASE: All LEDs red
        for (uint8_t i = led_min; i < led_max; i++) {
            rgb_matrix_set_color(i, 255, 0, 0);
        }
    } else if (cycle_pos < scan_time_per_half) {
        // FIRST HALF SCAN: Left half active
        if (is_left_half) {
            uint8_t target_led = cycle_pos / LED_CYCLE_TIME;
            if (target_led >= led_min && target_led < led_max) {
                if (target_led != last_printed_led) {
                    uprintf("LEFT scan: target=%u (batch %u-%u)\n",
                            target_led, led_min, led_max);
                    last_printed_led = target_led;
                }
                if (target_led < PERKEY_COUNT) {
                    rgb_matrix_set_color(target_led, 255, 255, 255);  // WHITE = per-key
                } else {
                    rgb_matrix_set_color(target_led, 0, 0, 255);      // BLUE = underglow
                }
            }
        }
    } else {
        // SECOND HALF SCAN: Right half active
        if (!is_left_half) {
            uint8_t local_target = (cycle_pos - scan_time_per_half) / LED_CYCLE_TIME;
            uint8_t target_led = LEDS_PER_HALF + local_target;
            if (target_led >= led_min && target_led < led_max) {
                if (target_led != last_printed_led) {
                    uprintf("RIGHT scan: target=%u local=%u (batch %u-%u)\n",
                            target_led, local_target, led_min, led_max);
                    last_printed_led = target_led;
                }
                if (local_target < PERKEY_COUNT) {
                    rgb_matrix_set_color(target_led, 255, 255, 255);  // WHITE = per-key
                } else {
                    rgb_matrix_set_color(target_led, 0, 0, 255);      // BLUE = underglow
                }
            }
        }
    }

    return false;
}
