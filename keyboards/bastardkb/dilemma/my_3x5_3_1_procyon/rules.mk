SERIAL_DRIVER = vendor
POINTING_DEVICE_DRIVER = digitizer
I2C_DRIVER_REQUIRED = yes

# Maxtouch digitizer
DIGITIZER_DRIVER = maxtouch
DIGITIZER_ENABLE = yes
MAXTOUCH_DEBUG = yes

MCU = RP2040
BOOTLOADER = rp2040
PLATFORM = chibios

# Features
# CONSOLE_ENABLE = yes # find it enabled in keyboard.json: `"console": true`
TRI_LAYER_ENABLE = yes
EE_HANDS = yes
ENCODER_MAP_ENABLE = yes
