/**
 * Copyright 2022 Charly Delay <charly@codesink.dev> (@0xcharly)
 * Copyright 2023 casuanoob <casuanoob@hotmail.com> (@casuanoob)
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

#pragma once

/* USB VBUS detection (not in JSON yet) */
// #define USB_VBUS_PIN GP19

/* Handedness - use EE_HANDS to store side in EEPROM */
#define EE_HANDS
// #define SPLIT_USB_DETECT
// #define SPLIT_HAND_PIN GP29
// #define SPLIT_HAND_PIN_LOW_IS_LEFT

/* Pointing device/digitizer are on the right half */
#define SPLIT_DIGITIZER_ENABLE
#define POINTING_DEVICE_RIGHT
#define DIGITIZER_RIGHT

/* Serial USART configuration (not in JSON yet) */
#define SERIAL_USART_RX_PIN GP1
#define SERIAL_USART_FULL_DUPLEX

/* Left-side matrix pins */
#define MATRIX_COL_PINS { GP8, GP7, GP5, GP13, GP9 }
#define MATRIX_ROW_PINS { GP6, GP12, GP18, GP17 }

/* Right-side matrix pins (not in JSON yet) */
#define MATRIX_COL_PINS_RIGHT { GP6, GP7, GP8, GP9, GP10 }
#define MATRIX_ROW_PINS_RIGHT { GP12, GP13, GP17, GP18 }

/* I2C for Maxtouch digitizer (not in JSON yet) */
#define I2C_DRIVER I2CD1
#define I2C_DRIVER_ID 1
#define I2C1_SDA_PIN GP2
#define I2C1_SCL_PIN GP3
#define I2C1_CLOCK_SPEED 1000000
#define DIGITIZER_MOTION_PIN GP11
#define DIGITIZER_MOTION_PIN_ACTIVE_LOW yes
#undef MXT_TOUCH_THRESHOLD
#define MXT_TOUCH_THRESHOLD 25
#undef MXT_MOVE_HYSTERESIS_INITIAL
#define MXT_MOVE_HYSTERESIS_INITIAL 15
#undef MXT_MOVE_HYSTERESIS_NEXT
#define MXT_MOVE_HYSTERESIS_NEXT 8
#define PROCYON_42_50

/* Bootloader double-tap (not in JSON yet) */
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
// #define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_LED GP17
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 500U

/* Optimization */
#define CRC8_USE_TABLE
#define CRC8_OPTIMIZE_SPEED

/* Dynamic keymap layers */
#define DYNAMIC_KEYMAP_LAYER_COUNT 4
