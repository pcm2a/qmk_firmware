VIA_ENABLE = yes
RGB_MATRIX_ENABLE = yes
ENCODER_MAP_ENABLE = yes
CONSOLE_ENABLE = no
RGB_MATRIX_CUSTOM_USER = no
TAP_DANCE_ENABLE = yes
WPM_ENABLE = yes
SRC += starfield.c
# SRC += matrix.c
CFLAGS += -Wno-error=unused-function
