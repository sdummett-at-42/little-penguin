obj-m += fortytwo.o

PWD := $(CURDIR)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean%

install: remove
	sudo insmod fortytwo.ko

remove:
	- sudo rmmod fortytwo.ko 2>/dev/null
