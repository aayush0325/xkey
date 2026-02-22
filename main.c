#include "xkey.h"

int keyboard_fd;
int xkey_fd;
volatile sig_atomic_t stop;
int toggled;

int main() {
	struct sigaction sa = {0};

	const char *dev =
	"/dev/input/by-path/pci-0000:06:00.3-usb-0:1:1.0-event-kbd";

	stop = 0;
	toggled = FALSE;

	sa.sa_handler = handle_signal;
	sigaction(SIGINT, &sa, NULL);

	keyboard_fd = open(dev, O_RDONLY);
	xkey_fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

	if (keyboard_fd < 0) {
        fprintf(stderr, "Cannot open %s: %s\n", dev, strerror(errno));
        return EXIT_FAILURE;
    }

	if (xkey_fd < 0) {
        fprintf(stderr, "CANNOT CREATE VIRTUAL DEVICE: %s", strerror(errno));
        return EXIT_FAILURE;
    }

	sleep(2);
	ioctl(keyboard_fd, EVIOCGRAB, FALSE);

	setup_virtual_device();

    while (1) {
		int res = handle_workflow();
		if (res < 0) {
			break;
		}
    }

	ioctl(keyboard_fd, EVIOCGRAB, FALSE);
    close(keyboard_fd);
	ioctl(xkey_fd, UI_DEV_DESTROY);
	close(xkey_fd);
    return EXIT_SUCCESS;
}
