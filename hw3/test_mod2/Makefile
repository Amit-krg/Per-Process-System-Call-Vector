obj-m += test_mod2.o

INC=/lib/modules/$(shell uname -r)/build/arch/x86/include

all: test_mod2

test_mod2:
	make -Wall -Werror -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
