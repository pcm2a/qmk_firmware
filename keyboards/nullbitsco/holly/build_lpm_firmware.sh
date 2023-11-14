#!/bin/bash -e

# This script is used to compile the holly low-power mode firmware using the pico-sdk. 
# It then exports it to 'flash.h' for inclusion in the QMK firmware.
# This does not need to be re-run unless the low-power firmware is changed.

# push into the script directory so that relative paths are correct
SCRIPT_PATH=$(dirname "$(realpath -s "$0")")
pushd "$SCRIPT_PATH" || exit 1
pushd holly_lpm || exit 1

# build the firmware
mkdir -p build && pushd build && cmake .. -DPICO_BOARD="holly"; make -j"$(nproc)"
LPM_FW_PATH="$(realpath holly_lpm.bin)"
popd || exit 1

# get the path to uf2conv.py
UF2CONV="$(realpath util/uf2conv.py)"
popd || exit 1
python3 "$UF2CONV" "$LPM_FW_PATH" --carray

# build the QMK firmware
# qmk compile -kb nullbitsco/holly -km via
