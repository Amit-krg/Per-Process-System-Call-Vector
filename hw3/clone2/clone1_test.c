#define _GNU_SOURCE
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sched.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../include/uapi/linux/sched.h"

#define err_exit(msg) do { perror(msg); exit(EXIT_FAILURE); \
			} while (0)

static int childfunc(void *arg)
{
	char in;

	printf("In childfunc\n");
	printf("Argument passed is:  %s\n", (char *)arg);
	mkdir("foo", 0777);
	printf("After mkdir(foo)\n");
	printf("Enter any char to continue\n");
	scanf("%c", &in);
	scanf("%c", &in);
	mkdir("bar", 0777);
	return 0;
}

#define STACK_SIZE (1024 * 1024 * 1024)

int main(int argc, char *argv[])
{
	char *stack;                    /* Start of stack buffer */
	char *stacktop;                 /* End of stack buffer */
	pid_t pid;
	char in;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s <child arg> <1/0>\n", argv[0]);
		exit(EXIT_SUCCESS);
	}

	stack = malloc(STACK_SIZE);
	if (!stack)
	err_exit("malloc");
	stacktop = stack + STACK_SIZE;  /* Assume stack grows downward */
	printf("PID is %d\n", getpid());
	printf("Enter any char to start\n");
	scanf("%c", &in);
	if (atoi(argv[2]) == 0)
		pid = clone(childfunc, stacktop, SIGCHLD, argv[1]);
	else
		pid = clone(childfunc, stacktop,
			    SIGCHLD | CLONE_INHERIT_VECTOR, argv[1]);
	if (pid == -1)
		err_exit("clone");
	printf("clone() returned %ld\n", (long)pid);

	/* Parent falls through to here */

	if (waitpid(pid, NULL, 0) == -1)    /* Wait for child */
		err_exit("waitpid");
	printf("child has terminated\n");

	exit(EXIT_SUCCESS);
}
