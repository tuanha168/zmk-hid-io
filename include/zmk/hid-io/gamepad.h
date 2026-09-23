#pragma once

#include <stdint.h>

#define ZMK_HID_GAMEPAD_NUM_BUTTONS 16

enum zmk_hid_gamepad_axis {
    ZMK_HID_GAMEPAD_AXIS_LX,
    ZMK_HID_GAMEPAD_AXIS_LY,
    ZMK_HID_GAMEPAD_AXIS_DPAD_X,
    ZMK_HID_GAMEPAD_AXIS_DPAD_Y,
    ZMK_HID_GAMEPAD_AXIS_COUNT,
};

enum zmk_hid_gamepad_direction {
    ZMK_HID_GAMEPAD_DIRECTION_NEGATIVE,
    ZMK_HID_GAMEPAD_DIRECTION_POSITIVE,
    ZMK_HID_GAMEPAD_DIRECTION_COUNT,
};

int zmk_hid_gamepad_axis_press(uint8_t axis, uint8_t direction);
int zmk_hid_gamepad_axis_release(uint8_t axis, uint8_t direction);
int zmk_hid_gamepad_button_press(uint8_t button);
int zmk_hid_gamepad_button_release(uint8_t button);
void zmk_hid_gamepad_right_stick_set(int16_t x, int16_t y);
