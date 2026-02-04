// SPDX-License-Identifier: GPL-2.0-or-later
#include "vial_password.h"

#include <string.h>

#include "aes.h"

static void vial_secure_zero(void *ptr, size_t len) {
    volatile uint8_t *p = (volatile uint8_t *)ptr;
    while (len--) {
        *p++ = 0;
    }
}

__attribute__((weak)) void vial_password_get_key(uint8_t key[VIAL_PASSWORD_KEY_SIZE]) {
    memset(key, 0, VIAL_PASSWORD_KEY_SIZE);
}

static uint8_t vial_password_key_cache[VIAL_PASSWORD_KEY_SIZE];
static bool vial_password_key_cached = false;

static void vial_password_load_key(uint8_t key[VIAL_PASSWORD_KEY_SIZE]) {
    if (!vial_password_key_cached) {
        vial_password_get_key(vial_password_key_cache);
        vial_password_key_cached = true;
    }

    memcpy(key, vial_password_key_cache, VIAL_PASSWORD_KEY_SIZE);
}

bool vial_password_decrypt_macro(const uint8_t *ciphertext, uint16_t len, const uint8_t iv[VIAL_PASSWORD_IV_SIZE], uint8_t *output) {
    if (ciphertext == NULL || output == NULL || iv == NULL) {
        return false;
    }

    if (len == 0) {
        return true;
    }

    memcpy(output, ciphertext, len);

    uint8_t key[VIAL_PASSWORD_KEY_SIZE];
    struct AES_ctx ctx;

    vial_password_load_key(key);
    AES_init_ctx_iv(&ctx, key, iv);
    AES_CTR_xcrypt_buffer(&ctx, output, len);

    vial_secure_zero(key, sizeof(key));
    vial_secure_zero(&ctx, sizeof(ctx));

    return true;
}

void vial_password_clear_state(void) {
    vial_secure_zero(vial_password_key_cache, sizeof(vial_password_key_cache));
    vial_password_key_cached = false;
}
