
#EXTRA_C = $1

INC=/lib/modules/$(shell uname -r)/build/arch/x86/include

all: vrctl

#file:
#gcc -Wall -Werror -I$(INC)/generated/uapi -I$(INC)/uapi treplay.c -o treplay list.c -o list.o

#treplay: treplay.o list.o
#	gcc  $(FLAG) -lssl -lcrypto -Wall -Werror -I$(INC)/generated/uapi -I$(INC)/uapi -o treplay treplay.o list.o

#treplay.o: treplay.c list.h
#	gcc $(FLAG) -lssl -lcrypto -Wall -Werror -I$(INC)/generated/uapi -I$(INC)/uapi -c treplay.c
#gcc -c main.c

#list.o: list.c list.h
#	gcc  -lssl -lcrypto -Wall -Werror -I$(INC)/generated/uapi -I$(INC)/uapi -c list.c

vrctl: vrctl.c
	gcc $(FLAG) -Wall -Werror -I$(INC)/generated/uapi -I$(INC)/uapi -o vrctl vrctl.c



#	gcc -Wall -Werror -I$(INC)/generated/uapi -I$(INC)/uapi list.c -o list
#xmergesort_mod:
#	make -Wall -Werror -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f vrctl
