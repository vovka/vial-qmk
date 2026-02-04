// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define VIAL_PASSWORD_KEY_SIZE 16
#define VIAL_PASSWORD_IV_SIZE 16

bool vial_password_decrypt_macro(const uint8_t *ciphertext, uint16_t len, const uint8_t iv[VIAL_PASSWORD_IV_SIZE], uint8_t *output);
void vial_password_clear_state(void);
void vial_password_get_key(uint8_t key[VIAL_PASSWORD_KEY_SIZE]);
