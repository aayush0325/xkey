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

void emit(int type, int code, int val) {
    struct input_event ie;
    ie.type = type;
    ie.code = code;
    ie.value = val;
    ie.time.tv_sec = 0; // Timestamps are typically ignored
    ie.time.tv_usec = 0;
    write(xkey_fd, &ie, sizeof(ie));
}

void emit_north(int val) { emit(EV_KEY, BTN_NORTH, val); }
void emit_south(int val) { emit(EV_KEY, BTN_SOUTH, val); }
void emit_east(int val) { emit(EV_KEY, BTN_EAST, val); }
void emit_west(int val) { emit(EV_KEY, BTN_WEST, val); }
void emit_tl(int val) { emit(EV_KEY, BTN_TL, val); }
void emit_tr(int val) { emit(EV_KEY, BTN_TR, val); }
void emit_hat0x(int val) { emit(EV_ABS, ABS_HAT0X, val); }
void emit_hat0y(int val) { emit(EV_ABS, ABS_HAT0Y, val); }
void emit_lz(int val) { emit(EV_ABS, ABS_Z, val); }
void emit_rz(int val) { emit(EV_ABS, ABS_RZ, val); }

void syn() {
	emit(EV_SYN, SYN_REPORT, 0);
}

void write_event(int event, int val) {
	if (val > 1 || val < 0) return;

	switch (event) {
		case KEY_I:
			printf("up key pressed, pressing north\n");
			emit_north(val);
			break;
		case KEY_K:
			printf("down key pressed, pressing south\n");
			emit_south(val);
			break;
		case KEY_J:
			printf("left key pressed, pressing west\n");
			emit_west(val);
			break;
		case KEY_L:
			printf("right key pressed, pressing east\n");
			emit_east(val);
			break;
		case KEY_LEFTCTRL:
			printf("lctrl key pressed, pressing tl\n");
			emit_tl(val);
			break;
		case KEY_RIGHTCTRL:
			printf("rctrl key pressed, pressing tr\n");
			emit_tr(val);
			break;
		case KEY_UP:
			printf("up arrow pressed, hat0y = -1\n");
			emit_hat0y(val ? -1 : 0);
			break;
		case KEY_DOWN:
			printf("down arrow pressed, hat0y = 1\n");
			emit_hat0y(val ? 1 : 0);
			break;
		case KEY_LEFT:
			printf("left arrow pressed, hat0x = -1\n");
			emit_hat0x(val ? -1 : 0);
			break;
		case KEY_RIGHT:
			printf("right arrow pressed, hat0x = 1\n");
			emit_hat0x(val ? 1 : 0);
			break;
		case KEY_LEFTSHIFT:
			printf("lshift pressed, lz = 255\n");
			emit_lz(val ? 255 : 0);
			break;
		case KEY_RIGHTSHIFT:
			printf("rshift pressed, rz = 255\n");
			emit_rz(val ? 255 : 0);
			break;
		default:
			return;
	}
}
