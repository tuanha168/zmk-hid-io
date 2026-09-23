#define DT_DRV_COMPAT zmk_behavior_hid_io_gamepad_button

#include <zephyr/device.h>
#include <drivers/behavior.h>

#include <zmk/behavior.h>
#include <zmk/hid-io/endpoints.h>
#include <zmk/hid-io/gamepad.h>

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

static int behavior_gamepad_button_init(const struct device *dev) { return 0; }

static int on_pressed(struct zmk_behavior_binding *binding,
                      struct zmk_behavior_binding_event event) {
    int err = zmk_hid_gamepad_button_press(binding->param1);
    if (err) {
        return err;
    }

    zmk_endpoints_send_gamepad_report();
    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_released(struct zmk_behavior_binding *binding,
                       struct zmk_behavior_binding_event event) {
    int err = zmk_hid_gamepad_button_release(binding->param1);
    if (err) {
        return err;
    }

    zmk_endpoints_send_gamepad_report();
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api behavior_gamepad_button_driver_api = {
    .binding_pressed = on_pressed,
    .binding_released = on_released,
};

#define GP_BUTTON_INST(n)                                                                          \
    BEHAVIOR_DT_INST_DEFINE(n, behavior_gamepad_button_init, NULL, NULL, NULL, POST_KERNEL,        \
                            CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,                                   \
                            &behavior_gamepad_button_driver_api);

DT_INST_FOREACH_STATUS_OKAY(GP_BUTTON_INST)

#endif
