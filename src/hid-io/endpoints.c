/*
 * Copyright (c) 2024 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <zmk/endpoints.h>
#include <zmk/hid.h>
#include <zmk/usb_hid.h>
#include <zmk/hog.h>

#include <zephyr/logging/log.h>
#include <zephyr/sys/util.h>
LOG_MODULE_DECLARE(hid_io, CONFIG_ZMK_HID_IO_LOG_LEVEL);

#include <dt-bindings/zmk/hid-io/gamepad.h>

#include <zmk/hid-io/endpoints.h>
#include <zmk/hid-io/hid.h>
#include <zmk/hid-io/usb_hid.h>
#include <zmk/hid-io/hog.h>

#ifndef ZMK_ENDPOINT_NONE_COUNT
struct zmk_endpoint_instance zmk_endpoint_get_selected(void) {
    return zmk_endpoint_selected();
}
#endif

#if IS_ENABLED(CONFIG_ZMK_HID_IO_GAMEPAD)
int zmk_endpoints_send_gamepad_report(void) {
    struct zmk_endpoint_instance current_instance = zmk_endpoint_get_selected();

    switch (current_instance.transport) {
#if IS_ENABLED(CONFIG_ZMK_USB)
    case ZMK_TRANSPORT_USB: {
        int err = zmk_usb_hid_send_gamepad_report();
        if (err) {
            LOG_ERR("FAILED TO SEND GAMEPAD OVER USB: %d", err);
        }
        return err;
    }
#else
    case ZMK_TRANSPORT_USB: break;
#endif /* IS_ENABLED(CONFIG_ZMK_USB) */

#if IS_ENABLED(CONFIG_ZMK_BLE)
    case ZMK_TRANSPORT_BLE: {
        struct zmk_hid_gamepad_report *gamepad_report = zmk_hid_get_gamepad_report();
        struct zmk_hid_gamepad_report_body report_body = gamepad_report->body;

        /* Android maps generic BLE buttons 4 and 5 opposite to desktop USB. */
        bool north_pressed = report_body.buttons & BIT(GP_NORTH);
        bool west_pressed = report_body.buttons & BIT(GP_WEST);

        WRITE_BIT(report_body.buttons, GP_NORTH, west_pressed);
        WRITE_BIT(report_body.buttons, GP_WEST, north_pressed);

        int err = zmk_hog_send_gamepad_report(&report_body);
        if (err) {
            LOG_ERR("FAILED TO SEND GAMEPAD OVER HOG: %d", err);
        }
        return err;
    }
#else
    case ZMK_TRANSPORT_BLE: break;
#endif /* IS_ENABLED(CONFIG_ZMK_BLE) */

#ifdef ZMK_ENDPOINT_NONE_COUNT
    case ZMK_TRANSPORT_NONE: return 0;
#endif
    }

    LOG_ERR("Unsupported endpoint transport %d", current_instance.transport);
    return -ENOTSUP;
}
#endif // IS_ENABLED(CONFIG_ZMK_HID_IO_GAMEPAD)

#if IS_ENABLED(CONFIG_ZMK_HID_IO_JOYSTICK)
int zmk_endpoints_send_joystick_report_alt() {
    struct zmk_endpoint_instance current_instance = zmk_endpoint_get_selected();

    switch (current_instance.transport) {
#if IS_ENABLED(CONFIG_ZMK_USB)
    case ZMK_TRANSPORT_USB: {
        int err = zmk_usb_hid_send_joystick_report_alt();
        if (err) {
            LOG_ERR("FAILED TO SEND OVER USB: %d", err);
        }
        return err;
    }
#else
    case ZMK_TRANSPORT_USB: break;
#endif /* IS_ENABLED(CONFIG_ZMK_USB) */

#if IS_ENABLED(CONFIG_ZMK_BLE)
    case ZMK_TRANSPORT_BLE: {
        struct zmk_hid_joystick_report_alt *joystick_report = zmk_hid_get_joystick_report_alt();
        int err = zmk_hog_send_joystick_report_alt(&joystick_report->body);
        if (err) {
            LOG_ERR("FAILED TO SEND OVER HOG: %d", err);
        }
        return err;
    }
#else
    case ZMK_TRANSPORT_BLE: break;
#endif /* IS_ENABLED(CONFIG_ZMK_BLE) */

#ifdef ZMK_ENDPOINT_NONE_COUNT
    case ZMK_TRANSPORT_NONE:  {
        LOG_ERR("Current endpoint transport: NONE");
        return 0;
    }
#endif

    }

    LOG_ERR("Unsupported endpoint transport %d", current_instance.transport);
    return -ENOTSUP;
}
#endif // IS_ENABLED(CONFIG_ZMK_HID_IO_JOYSTICK)


#if IS_ENABLED(CONFIG_ZMK_HID_IO_MOUSE)
int zmk_endpoints_send_mouse_report_alt() {
    struct zmk_endpoint_instance current_instance = zmk_endpoint_get_selected();

    switch (current_instance.transport) {
#if IS_ENABLED(CONFIG_ZMK_USB)
    case ZMK_TRANSPORT_USB: {
        int err = zmk_usb_hid_send_mouse_report_alt();
        if (err) {
            LOG_ERR("FAILED TO SEND OVER USB: %d", err);
        }
        return err;
    }
#else
    case ZMK_TRANSPORT_USB: break;
#endif /* IS_ENABLED(CONFIG_ZMK_USB) */

#if IS_ENABLED(CONFIG_ZMK_BLE)
    case ZMK_TRANSPORT_BLE: {
        struct zmk_hid_mouse_report_alt *mouse_report = zmk_hid_get_mouse_report_alt();
        int err = zmk_hog_send_mouse_report_alt(&mouse_report->body);
        if (err) {
            LOG_ERR("FAILED TO SEND OVER HOG: %d", err);
        }
        return err;
    }
#else
    case ZMK_TRANSPORT_BLE: break;
#endif /* IS_ENABLED(CONFIG_ZMK_BLE) */

#ifdef ZMK_ENDPOINT_NONE_COUNT
    case ZMK_TRANSPORT_NONE: return 0;
#endif
    }

    LOG_ERR("Unsupported endpoint transport %d", current_instance.transport);
    return -ENOTSUP;
}
#endif // IS_ENABLED(CONFIG_ZMK_HID_IO_MOUSE)


#if IS_ENABLED(CONFIG_ZMK_HID_IO_VOLUME_KNOB)
int zmk_endpoints_send_volume_knob_report_alt() {
    struct zmk_endpoint_instance current_instance = zmk_endpoint_get_selected();

    switch (current_instance.transport) {
#if IS_ENABLED(CONFIG_ZMK_USB)
    case ZMK_TRANSPORT_USB: {
        int err = zmk_usb_hid_send_volume_knob_report_alt();
        if (err) {
            LOG_ERR("FAILED TO SEND OVER USB: %d", err);
        }
        return err;
    }
#else
    case ZMK_TRANSPORT_USB: break;
#endif /* IS_ENABLED(CONFIG_ZMK_USB) */

#if IS_ENABLED(CONFIG_ZMK_BLE)
    case ZMK_TRANSPORT_BLE: {
        struct zmk_hid_volume_knob_report_alt *volume_knob_report = zmk_hid_get_volume_knob_report_alt();
        int err = zmk_hog_send_volume_knob_report_alt(&volume_knob_report->body);
        if (err) {
            LOG_ERR("FAILED TO SEND OVER HOG: %d", err);
        }
        return err;
    }
#else
    case ZMK_TRANSPORT_BLE: break;
#endif /* IS_ENABLED(CONFIG_ZMK_BLE) */

#ifdef ZMK_ENDPOINT_NONE_COUNT
    case ZMK_TRANSPORT_NONE: return 0;
#endif
    }

    LOG_ERR("Unsupported endpoint transport %d", current_instance.transport);
    return -ENOTSUP;
}
#endif // IS_ENABLED(CONFIG_ZMK_HID_IO_VOLUME_KNOB)
