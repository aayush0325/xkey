#ifndef XKEY_H
#define XKEY_H

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <termios.h>
#include <unistd.h>

#define IS_PRESSED 1
#define IS_RELEASED 0
#define FALSE 0
#define TRUE 1

extern int keyboard_fd;
extern int xkey_fd;
extern volatile sig_atomic_t stop;
extern int toggled;

void handle_signal(int sig);

void setup_virtual_device();

int handle_workflow();

void write_event(int event, int val);

void emit(int type, int code, int val);

void syn();

int list_devices();

#endif
