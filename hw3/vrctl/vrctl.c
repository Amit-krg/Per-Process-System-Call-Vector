#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>

#define PATH "/tmp"

#define SET_VECTOR _IO(0xa1, 1)
#define GET_VECTOR _IO(0xa1, 2)
#define GET_VECTOR_LIST_SIZE _IO(0xa1, 3)
#define GET_VECTOR_LIST _IO(0xa1, 4)

struct ioctl_arguments {
	int pid;
	int vid;
};

struct vector_list {
	int list_size;
	int *list;
};

int main(int argc, char **argv)
{
	int fd;
	struct ioctl_arguments args;

	if (argc > 3) {                                                          /*checks for missing argunent*/
		printf("ERROR : Missing arguments\n");
		printf("Usage: %s PID VID\n", argv[0]);
		exit(1);
	}

	fd = open(PATH, O_RDONLY);

	if (argc == 2) {
		args.pid = atoi(argv[1]);

		if (ioctl(fd, GET_VECTOR, &args) == -1)
			printf("Vector get failed: %s\n", strerror(errno));
		else
			printf("Vector Id is %d\n", args.vid);

	} else if (argc == 3) {
		args.pid = atoi(argv[1]);
		args.vid = atoi(argv[2]);

		if (ioctl(fd, SET_VECTOR, &args) == -1)
			printf("Vector Set failed: %s\n", strerror(errno));
	} else {
		int i;
		struct vector_list vector_list1;
		int vector_list_size = 0;


		if (ioctl(fd, GET_VECTOR_LIST_SIZE, &vector_list_size) == -1)
			printf("Vector size get failed: %s\n", strerror(errno));

		if (vector_list_size) {
			vector_list1.list_size = vector_list_size;
			vector_list1.list = (int *)malloc(sizeof(int) * vector_list_size * 2);

			if (!vector_list1.list)
				printf("Vector list malloc failed\n");
			if (ioctl(fd, GET_VECTOR_LIST, &vector_list1) == -1)
				printf("Vector list get failed: %s\n", strerror(errno));
			if (vector_list1.list) {
				printf("Vid,  rc_count\n");
				for (i = 0; i < vector_list1.list_size * 2; i++) {
					printf("%d,    %d\n", vector_list1.list[i], vector_list1.list[i+1]);
					i++;
				}
				free(vector_list1.list);
			}
		} else
			printf("Vector list is empty\n");
	}

	close(fd);
	return 0;
}
