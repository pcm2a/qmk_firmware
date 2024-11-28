# HOLLY

![HOLLY](https://nullbits.co/static/img/holly1.jpg)

A 6-switch RP2040 macropad and ornament built by nullbits.

Insert a CR2032 coin cell into the coin cell holder and slide the switch to the left (away from the USB connector) to enter ornament mode. The LEDs will sparkle for several hours before shutting off to conserve power.

In order to enter the bootloader, hold switch #1 while plugging in the USB cable.

To build in QMK, clone or symlink the `holly` directory into your QMK firmware directory: `keyboards/nullbitsco/`.
Then, build with `qmk compile -kb nullbitsco/holly -km all`.
