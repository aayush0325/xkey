#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <string.h>
#include <stdio.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <linux/uinput.h>
#include <sys/time.h>

static const char *evval[] = {
    "RELEASE",
    "PRESS",
    "REPEAT"
};

volatile sig_atomic_t stop = 0;

void handle_signal(int sig) {
	(void)sig;
	stop = 1;
}

int main() {
    const char *dev =
		"/dev/input/by-path/pci-0000:06:00.3-usb-0:1:1.0-event-kbd";

    struct input_event ev;
    ssize_t n;
	int keyboard_fd;

	static int xkey_fd = -1;
	struct uinput_setup usetup;
	struct input_event event;

    keyboard_fd = open(dev, O_RDONLY);
	xkey_fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

	memset(&usetup, 0, sizeof(usetup));

	snprintf(usetup.name, UINPUT_MAX_NAME_SIZE, "XKEY-controller");
    usetup.id.version = 1;
    usetup.id.vendor = 0x1;
    usetup.id.product = 0x1;
    usetup.id.bustype = BUS_USB;

	struct uinput_abs_setup abs = {0};
	abs.code = ABS_X;
	abs.absinfo.minimum = -32768;
	abs.absinfo.maximum = 32767;

	ioctl(xkey_fd, UI_ABS_SETUP, &abs);

	struct sigaction sa = {0};
	sa.sa_handler = handle_signal;
	sigaction(SIGINT, &sa, NULL);

	if (keyboard_fd < 0) {
        fprintf(stderr, "Cannot open %s: %s\n", dev, strerror(errno));
        return EXIT_FAILURE;
    }

	if (xkey_fd < 0) {
        fprintf(stderr, "CANNOT CREATE VIRTUAL DEVICE: %s", strerror(errno));
        return EXIT_FAILURE;
    }

	sleep(2);
	ioctl(keyboard_fd, EVIOCGRAB, 1);

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

	int toggled = 0;

	int is_esc_pressed = 0;

    while (1) {
		if (stop) {
			break;
		}
        n = read(keyboard_fd, &ev, sizeof(ev));

        if (n == -1) {
            if (errno == EINTR || errno == EAGAIN)
                continue;
            perror("read");
            break;
        }

        if (n != sizeof(ev)) {
            fprintf(stderr, "Partial read\n");
            break;
        }

		if (ev.type == EV_SYN && ev.code == SYN_REPORT) {
            printf("--------SYN REPORT--------\n");
            fflush(stdout);
			continue;
        }

        if (ev.type == EV_KEY && ev.value >= 0 && ev.value <= 2) {
            printf("%s  code=0x%04x (%d)\n",
                   evval[ev.value],
                   ev.code,
                   ev.code);
        }

		if (ev.type == EV_KEY && ev.code == KEY_ESC && ev.value == 1) {
			is_esc_pressed = 1;
		}

		if (ev.type == EV_KEY && ev.code == KEY_ESC && ev.value == 0 && is_esc_pressed) {
			is_esc_pressed = 0;
			if (toggled) {
				ioctl(keyboard_fd, EVIOCGRAB, 0);
				toggled = 0;
			} else {
				ioctl(keyboard_fd, EVIOCGRAB, 1);
				toggled = 1;
			}
		}
    }

	ioctl(keyboard_fd, EVIOCGRAB, 0);
    close(keyboard_fd);
	close(xkey_fd);
	ioctl(xkey_fd, UI_DEV_DESTROY);
    return EXIT_SUCCESS;
}
