#include "xkey.h"


static inline void SET_ABS(int axis, int min, int max, int fuzz, int flat) {
    struct uinput_abs_setup abs_setup;
	memset(&abs_setup, 0, sizeof(abs_setup));
	abs_setup.code = axis;
	abs_setup.absinfo.minimum = min;
	abs_setup.absinfo.maximum = max;
	abs_setup.absinfo.fuzz    = fuzz;
	abs_setup.absinfo.flat    = flat;
	ioctl(xkey_fd, UI_ABS_SETUP, &abs_setup);
}

void setup_virtual_device() {
    struct uinput_setup usetup;

    memset(&usetup, 0, sizeof(usetup));

    snprintf(usetup.name, UINPUT_MAX_NAME_SIZE, "Microsoft X-Box 360 pad");

    usetup.id.bustype = BUS_USB;
    usetup.id.vendor  = 0x45e;
    usetup.id.product = 0x28e;
    usetup.id.version = 0x110;
	usetup.ff_effects_max = 16;

	// Event types
    ioctl(xkey_fd, UI_SET_EVBIT, EV_KEY);
    ioctl(xkey_fd, UI_SET_EVBIT, EV_ABS);
    ioctl(xkey_fd, UI_SET_EVBIT, EV_SYN);
	ioctl(xkey_fd, UI_SET_EVBIT, EV_FF);

    // Main face buttons
    ioctl(xkey_fd, UI_SET_KEYBIT, BTN_SOUTH);
    ioctl(xkey_fd, UI_SET_KEYBIT, BTN_EAST);
    ioctl(xkey_fd, UI_SET_KEYBIT, BTN_NORTH);
    ioctl(xkey_fd, UI_SET_KEYBIT, BTN_WEST);

    // Shoulders
    ioctl(xkey_fd, UI_SET_KEYBIT, BTN_TL);
    ioctl(xkey_fd, UI_SET_KEYBIT, BTN_TR);

    // Stick press
    ioctl(xkey_fd, UI_SET_KEYBIT, BTN_THUMBL);
    ioctl(xkey_fd, UI_SET_KEYBIT, BTN_THUMBR);

    // Center buttons
    ioctl(xkey_fd, UI_SET_KEYBIT, BTN_SELECT);
    ioctl(xkey_fd, UI_SET_KEYBIT, BTN_START);
    ioctl(xkey_fd, UI_SET_KEYBIT, BTN_MODE);

	// Axes
    ioctl(xkey_fd, UI_SET_ABSBIT, ABS_X);      // Left stick X
    ioctl(xkey_fd, UI_SET_ABSBIT, ABS_Y);      // Left stick Y
    ioctl(xkey_fd, UI_SET_ABSBIT, ABS_RX);     // Right stick X
    ioctl(xkey_fd, UI_SET_ABSBIT, ABS_RY);     // Right stick Y
    ioctl(xkey_fd, UI_SET_ABSBIT, ABS_Z);      // Left trigger
    ioctl(xkey_fd, UI_SET_ABSBIT, ABS_RZ);     // Right trigger
    ioctl(xkey_fd, UI_SET_ABSBIT, ABS_HAT0X);  // D-pad X
    ioctl(xkey_fd, UI_SET_ABSBIT, ABS_HAT0Y);  // D-pad Y

    // Stick axes (centered)
    int stick_min = -32768;
    int stick_max =  32767;

    int trigger_min = 0;
    int trigger_max = 255;

    int hat_min = -1;
    int hat_max = 1;

    // Sticks
    SET_ABS(ABS_X,  stick_min, stick_max, 16, 128);
    SET_ABS(ABS_Y,  stick_min, stick_max, 16, 128);
    SET_ABS(ABS_RX, stick_min, stick_max, 16, 128);
    SET_ABS(ABS_RY, stick_min, stick_max, 16, 128);

    // Triggers
    SET_ABS(ABS_Z,  trigger_min, trigger_max, 0, 0);
    SET_ABS(ABS_RZ, trigger_min, trigger_max, 0, 0);

    // D-Pad as Hat
    SET_ABS(ABS_HAT0X, hat_min, hat_max, 0, 0);
    SET_ABS(ABS_HAT0Y, hat_min, hat_max, 0, 0);

	// Force feedback
	ioctl(xkey_fd, UI_SET_FFBIT, FF_RUMBLE);
	ioctl(xkey_fd, UI_SET_FFBIT, FF_PERIODIC);
	ioctl(xkey_fd, UI_SET_FFBIT, FF_SQUARE);
	ioctl(xkey_fd, UI_SET_FFBIT, FF_TRIANGLE);
	ioctl(xkey_fd, UI_SET_FFBIT, FF_SINE);
	ioctl(xkey_fd, UI_SET_FFBIT, FF_GAIN);
    ioctl(xkey_fd, UI_DEV_SETUP, &usetup);
    ioctl(xkey_fd, UI_DEV_CREATE);

    sleep(1); // give kernel time
}
