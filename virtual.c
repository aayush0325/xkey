#include "xkey.h"

void setup_virtual_device() {
	struct uinput_setup usetup;
	struct uinput_abs_setup abs = {0};

	memset(&usetup, 0, sizeof(usetup));

	snprintf(usetup.name, UINPUT_MAX_NAME_SIZE, "XKEY-controller");
    usetup.id.version = 1;
    usetup.id.vendor = 0x1;
    usetup.id.product = 0x1;
    usetup.id.bustype = BUS_USB;

	abs.code = ABS_X;
	abs.absinfo.minimum = -32768;
	abs.absinfo.maximum = 32767;

	ioctl(xkey_fd, UI_ABS_SETUP, &abs);

	memset(&usetup, 0, sizeof(usetup));

	snprintf(usetup.name, UINPUT_MAX_NAME_SIZE, "XKEY-controller");
    usetup.id.version = 1;
    usetup.id.vendor = 0x1;
    usetup.id.product = 0x1;
    usetup.id.bustype = BUS_USB;

	abs.code = ABS_X;
	abs.absinfo.minimum = -32768;
	abs.absinfo.maximum = 32767;

	ioctl(xkey_fd, UI_ABS_SETUP, &abs);

	ioctl(xkey_fd, UI_SET_KEYBIT, BTN_SOUTH);
	ioctl(xkey_fd, UI_SET_KEYBIT, BTN_EAST);
	ioctl(xkey_fd, UI_SET_KEYBIT, BTN_NORTH);
	ioctl(xkey_fd, UI_SET_KEYBIT, BTN_WEST);

	ioctl(xkey_fd, UI_SET_KEYBIT, BTN_TL);
	ioctl(xkey_fd, UI_SET_KEYBIT, BTN_TR);
	ioctl(xkey_fd, UI_SET_KEYBIT, BTN_TL2);
	ioctl(xkey_fd, UI_SET_KEYBIT, BTN_TR2);

	ioctl(xkey_fd, UI_SET_KEYBIT, BTN_SELECT);
	ioctl(xkey_fd, UI_SET_KEYBIT, BTN_START);
	ioctl(xkey_fd, UI_SET_KEYBIT, BTN_MODE);

	ioctl(xkey_fd, UI_SET_KEYBIT, BTN_THUMBL);
	ioctl(xkey_fd, UI_SET_KEYBIT, BTN_THUMBR);

	ioctl(xkey_fd, UI_SET_KEYBIT, BTN_DPAD_UP);
	ioctl(xkey_fd, UI_SET_KEYBIT, BTN_DPAD_DOWN);
	ioctl(xkey_fd, UI_SET_KEYBIT, BTN_DPAD_LEFT);
	ioctl(xkey_fd, UI_SET_KEYBIT, BTN_DPAD_RIGHT);

	ioctl(xkey_fd, UI_SET_ABSBIT, ABS_X);
	ioctl(xkey_fd, UI_SET_ABSBIT, ABS_Y);
	ioctl(xkey_fd, UI_SET_ABSBIT, ABS_RX);
	ioctl(xkey_fd, UI_SET_ABSBIT, ABS_RY);
	ioctl(xkey_fd, UI_SET_ABSBIT, ABS_Z);
	ioctl(xkey_fd, UI_SET_ABSBIT, ABS_RZ);

	ioctl(xkey_fd, UI_SET_EVBIT, EV_KEY);
	ioctl(xkey_fd, UI_SET_EVBIT, EV_ABS);
	ioctl(xkey_fd, UI_SET_EVBIT, EV_SYN);

	ioctl(xkey_fd, UI_DEV_SETUP, &usetup);
	ioctl(xkey_fd, UI_DEV_CREATE);
}
