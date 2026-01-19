SERIAL_DRIVER = vendor
I2C_DRIVER_REQUIRED = yes
POINTING_DEVICE_DRIVER = digitizer
POINTING_DEVICE_ENABLE = yes
SPLIT_POINTING_ENABLE = yes

# Digitizer (Procyon Maxtouch)
DIGITIZER_DRIVER = maxtouch
DIGITIZER_ENABLE = yes
SPLIT_DIGITIZER_ENABLE = yes

MAXTOUCH_DEBUG = no

MCU = RP2040
BOOTLOADER = rp2040
PLATFORM = chibios

# Features
# CONSOLE_ENABLE = yes # find it enabled in keyboard.json: `"console": true`
TRI_LAYER_ENABLE = yes
EE_HANDS = yes
ENCODER_MAP_ENABLE = yes
