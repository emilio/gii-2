#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <macros/fatal_error.h>
#include <macros/ensure_success.h>

#define ESC

#define TERM_RESET					ESC "c"
#define TERM_CLEAR					ESC "[H" ESC "[J"
#define TERM_ENABLE_LINE_WRAP		ESC "[7h"
#define TERM_DISABLE_LINE_WRAP		ESC "[7l"


typedef struct winsize term_size_t;

term_size_t term_size() {
	term_size_t ret;

	if ( ! isatty(STDOUT_FILENO) )
		FATAL_ERROR("%d is not a TTY", STDOUT_FILENO);

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &ret);

	return ret;
}

// void term_

void set_terminal_config() {
	struct termios config;

	if ( ! isatty(STDIN_FILENO) )
		FATAL_ERROR("%d is not a tty", STDIN_FILENO);

	ENSURE_SUCCESS(tcgetattr(STDIN_FILENO, &config));

	/** Disable canonical mode and echo */
	config.c_lflag &= ~(ICANON | ECHO);
	config.c_cc[VMIN] = 1;

	ENSURE_SUCCESS(tcsetattr(STDIN_FILENO, TCSAFLUSH, &config));
}

char keyboard_status() {
	char ret;

	if ( read(STDIN_FILENO, &ret, 1) != 1 )
		FATAL_ERROR("Read failed");

	return ret;
}

int main(int argc, char** argv) {
	char c;

	set_terminal_config();

	while ( (c = keyboard_status()) != 'q' )
		write(STDOUT_FILENO, &c, 1);

	return 0;
}
