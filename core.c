#include "xkey.h"

static const char *evval[] = {
    "RELEASE",
    "PRESS",
    "REPEAT"
};

int handle_workflow(int *is_esc_pressed, int *toggled) {
	struct input_event ev;
	int n;

	if (stop) {
		return -1;
	}

	n = read(keyboard_fd, &ev, sizeof(ev));

	if (n == -1) {
		if (errno == EINTR || errno == EAGAIN)
			return -1;
		perror("read");
	}

	if (n != sizeof(ev)) {
		fprintf(stderr, "Partial read\n");
		return -1;
	}

	if (ev.type == EV_SYN && ev.code == SYN_REPORT) {
		printf("--------SYN REPORT--------\n");
		fflush(stdout);
		return 1;
	}

	if (ev.type == EV_KEY && ev.value >= 0 && ev.value <= 2) {
		printf("%s  code=0x%04x (%d)\n",
			   evval[ev.value],
			   ev.code,
			   ev.code);
	}

	if (ev.type == EV_KEY && ev.code == KEY_ESC && ev.value == IS_PRESSED) {
		*is_esc_pressed = FALSE;
	}

	if (ev.type == EV_KEY && ev.code == KEY_ESC && ev.value == IS_RELEASED && *is_esc_pressed) {
		*is_esc_pressed = FALSE;
		if (*toggled) {
			ioctl(keyboard_fd, EVIOCGRAB, FALSE);
			*toggled = FALSE;
		} else {
			ioctl(keyboard_fd, EVIOCGRAB, TRUE);
			*toggled = TRUE;
		}
	}
	return 1;
}