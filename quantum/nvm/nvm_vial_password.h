// Copyright 2024 Nick Brassel (@tzarc)
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <stdint.h>

#ifndef VIAL_PASSWORD_SALT_SIZE
#    define VIAL_PASSWORD_SALT_SIZE 32
#endif

/**
 * @brief Reads the Vial password salt from NVM.
 * @param salt Buffer for the salt, must be at least VIAL_PASSWORD_SALT_SIZE bytes.
 */
void nvm_vial_password_get_salt(uint8_t *salt);
/**
 * @brief Writes the Vial password salt to NVM.
 * @param salt Buffer with the salt, must be at least VIAL_PASSWORD_SALT_SIZE bytes.
 */
void nvm_vial_password_set_salt(const uint8_t *salt);
