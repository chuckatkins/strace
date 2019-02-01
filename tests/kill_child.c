/**
 * Check for a corner case that previously lead to segfault due to an attempt
 * to access unitialised tcp->s_ent.
 *
 * 13994 ????( <unfinished ...>
 * ...
 * 13994 <... ???? resumed>) = ?
 */

#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "tests.h"

#define ITERS    10000
#define SC_ITERS 10000

int main(void)
{
	unsigned int i;

	for (i = 0; i < ITERS; i++) {
		int pid = fork();

		if (pid < 0)
			perror_msg_and_fail("fork");

		if (!pid) {
			unsigned int j;

			for (j = 0; j < SC_ITERS; j++)
				getuid();

			pause();

			return 0;
		}

		kill(pid, SIGKILL);
		wait(NULL);
	}

	return 0;
}
