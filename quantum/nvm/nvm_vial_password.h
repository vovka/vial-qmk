// Copyright 2024 Nick Brassel (@tzarc)
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <stdint.h>

#ifndef VIAL_PASSWORD_SALT_SIZE
#    define VIAL_PASSWORD_SALT_SIZE 32
#endif

void nvm_vial_password_get_salt(uint8_t *salt);
void nvm_vial_password_set_salt(const uint8_t *salt);
