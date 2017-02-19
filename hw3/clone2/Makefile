INC=/lib/modules/$(shell uname -r)/build/arch/x86/include

all: clone_test1 clone_test2

clone_test1: clone1_test.c
	gcc -Wall -Werror -I$(INC)/generated/uapi -I$(INC)/uapi clone1_test.c -o clone1_test

clone_test2: clone2_test.c __syscall_error.c clone2.S
	gcc -Wall -Werror -I$(INC)/generated/uapi -I$(INC)/uapi clone2_test.c __syscall_error.c clone2.S -o clone2_test

clean:
	rm -f clone1_test clone2_test
