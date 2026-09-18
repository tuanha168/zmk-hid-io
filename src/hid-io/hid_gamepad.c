#include <errno.h>
#include <stdint.h>

#include <zephyr/sys/util.h>

#include <zmk/hid-io/gamepad.h>
#include <zmk/hid-io/hid.h>
#include <zmk/hid-io/hid_gamepad.h>

static struct zmk_hid_gamepad_report gamepad_report = {
    .report_id = ZMK_HID_REPORT_ID__IO_GAMEPAD,
};

static uint8_t axis_press_counts[ZMK_HID_GAMEPAD_AXIS_COUNT]
                                [ZMK_HID_GAMEPAD_DIRECTION_COUNT];
static uint8_t button_press_counts[ZMK_HID_GAMEPAD_NUM_BUTTONS];

static int8_t axis_value(uint8_t axis) {
    bool negative = axis_press_counts[axis][ZMK_HID_GAMEPAD_DIRECTION_NEGATIVE] > 0;
    bool positive = axis_press_counts[axis][ZMK_HID_GAMEPAD_DIRECTION_POSITIVE] > 0;

    if (negative == positive) {
        return 0;
    }

    return negative ? -127 : 127;
}

static void update_axis(uint8_t axis) {
    if (axis == ZMK_HID_GAMEPAD_AXIS_LX) {
        gamepad_report.body.x = axis_value(axis);
    } else {
        gamepad_report.body.y = axis_value(axis);
    }
}

int zmk_hid_gamepad_axis_press(uint8_t axis, uint8_t direction) {
    if (axis >= ZMK_HID_GAMEPAD_AXIS_COUNT ||
        direction >= ZMK_HID_GAMEPAD_DIRECTION_COUNT) {
        return -EINVAL;
    }

    if (axis_press_counts[axis][direction] == UINT8_MAX) {
        return -EOVERFLOW;
    }

    axis_press_counts[axis][direction]++;
    update_axis(axis);
    return 0;
}

int zmk_hid_gamepad_axis_release(uint8_t axis, uint8_t direction) {
    if (axis >= ZMK_HID_GAMEPAD_AXIS_COUNT ||
        direction >= ZMK_HID_GAMEPAD_DIRECTION_COUNT) {
        return -EINVAL;
    }

    if (axis_press_counts[axis][direction] == 0) {
        return -EINVAL;
    }

    axis_press_counts[axis][direction]--;
    update_axis(axis);
    return 0;
}

int zmk_hid_gamepad_button_press(uint8_t button) {
    if (button >= ZMK_HID_GAMEPAD_NUM_BUTTONS) {
        return -EINVAL;
    }

    if (button_press_counts[button] == UINT8_MAX) {
        return -EOVERFLOW;
    }

    button_press_counts[button]++;
    WRITE_BIT(gamepad_report.body.buttons, button, true);
    return 0;
}

int zmk_hid_gamepad_button_release(uint8_t button) {
    if (button >= ZMK_HID_GAMEPAD_NUM_BUTTONS || button_press_counts[button] == 0) {
        return -EINVAL;
    }

    button_press_counts[button]--;
    if (button_press_counts[button] == 0) {
        WRITE_BIT(gamepad_report.body.buttons, button, false);
    }
    return 0;
}

struct zmk_hid_gamepad_report *zmk_hid_get_gamepad_report(void) {
    return &gamepad_report;
}
