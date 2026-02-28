#include "xkey.h"

void handle_signal(int sig)
{
	(void)sig;
	stop = 1;
}

int list_devices()
{
	struct dirent *de;
	DIR *dr = opendir("/dev/input");
	if (dr == NULL) {
		perror("Could not open /dev/input");
		return -1;
	}

	printf("Available input devices:\n");
	printf("%-10s %-20s %s\n", "Event", "Name", "Path");
	printf(
	    "------------------------------------------------------------\n");

	while ((de = readdir(dr)) != NULL) {
		if (strncmp(de->d_name, "event", 5) == 0) {
			char path[512];
			char name[256] = "Unknown";
			snprintf(path, sizeof(path), "/dev/input/%s",
				 de->d_name);
			int fd = open(path, O_RDONLY);
			if (fd >= 0) {
				ioctl(fd, EVIOCGNAME(sizeof(name)), name);
				close(fd);
			}
			printf("%-10s %-20s %s\n", de->d_name + 5, name, path);
		}
	}

	closedir(dr);
	return 0;
}
