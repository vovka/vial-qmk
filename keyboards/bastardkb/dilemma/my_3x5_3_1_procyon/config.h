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
#define USB_VBUS_PIN GP19

/* Handedness (not in JSON yet) */
// #define SPLIT_USB_DETECT
// #define SPLIT_HAND_PIN GP29
// #define SPLIT_HAND_PIN_LOW_IS_LEFT

/* Serial USART configuration (not in JSON yet) */
#define SERIAL_USART_RX_PIN GP1
#define SERIAL_USART_FULL_DUPLEX

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
#define PROCYON_42_50

/* Bootloader double-tap (not in JSON yet) */
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_LED GP17
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 500U

/* Optimization */
#define CRC8_USE_TABLE
#define CRC8_OPTIMIZE_SPEED

/* Dynamic keymap layers */
#define DYNAMIC_KEYMAP_LAYER_COUNT 4
