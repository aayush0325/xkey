#include "xkey.h"

void handle_signal(int sig)
{
	(void)sig;
	stop = 1;
}