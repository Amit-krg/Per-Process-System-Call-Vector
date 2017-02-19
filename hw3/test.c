#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <sys/stat.h>
#include <sys/types.h>
#include <stdbool.h>
#include <openssl/md5.h>

int main(int argc, char **argv)
{

	int ret =0;
	int fd;
	char buf[100];
	sleep(5);
	int i = 0;
	while(i < 10)
	{
		fd = open("/test",O_RDWR|O_CREAT, 0x1A4);
		ret = mkdir("/test", 0x1ED);
		ret = rmdir("/test");
		ret = read(fd, buf, 16);
		ret = write(fd, buf, 16);
		ret = link("/test","/test_link");
        	ret = symlink("/test","/test_sym");
		ret = rename("/test","/testcp");
		ret = rename("/testcp","/test");
        	ret = unlink("/test");
		close(fd);
		sleep(7);
		i++;
	}
	return ret;
}
