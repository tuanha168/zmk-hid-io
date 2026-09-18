#pragma once

#include <stdint.h>

struct zmk_hid_gamepad_report_body {
    int8_t x;
    int8_t y;
    int8_t rx;
    int8_t ry;
    uint16_t buttons;
} __packed;

struct zmk_hid_gamepad_report {
    uint8_t report_id;
    struct zmk_hid_gamepad_report_body body;
} __packed;

struct zmk_hid_gamepad_report *zmk_hid_get_gamepad_report(void);
