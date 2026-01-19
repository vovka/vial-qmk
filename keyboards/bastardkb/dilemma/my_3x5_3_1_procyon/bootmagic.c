#include "bootmagic.h"
#include "matrix.h"

/* This override makes Bootmagic honor both halves' keys without requiring a
 * hardware reset pin. Keep it around when you need both sides to bootloader jump
 * independently, and include this source via rules.mk (`SRC += $(KEYBOARD_PATH_1)/bootmagic.c`) to enable it.
 */
bool bootmagic_should_reset(void) {
    bool reset = matrix_get_row(BOOTMAGIC_ROW) & (1 << BOOTMAGIC_COLUMN);

#if defined(SPLIT_KEYBOARD) && defined(BOOTMAGIC_ROW_RIGHT) && defined(BOOTMAGIC_COLUMN_RIGHT)
    reset |= matrix_get_row(BOOTMAGIC_ROW_RIGHT) & (1 << BOOTMAGIC_COLUMN_RIGHT);
#endif

    return reset;
}
