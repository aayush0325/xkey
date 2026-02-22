#ifndef XKEY_H
#define XKEY_H

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

#define IS_PRESSED 1
#define IS_RELEASED 0

#define FALSE 0
#define TRUE 1

extern int keyboard_fd;
extern int xkey_fd;
extern volatile sig_atomic_t stop;

void handle_signal(int sig);

void setup_virtual_device();

int handle_workflow(int *is_esc_pressed, int *toggled);

#endif