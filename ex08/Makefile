obj-m += reverse.o

PWD := $(CURDIR)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean%

install: remove
	sudo insmod reverse.ko

remove:
	- sudo rmmod reverse.ko 2>/dev/null
