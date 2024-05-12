obj-m += mymounts.o

PWD := $(CURDIR)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean%

install: remove
	sudo insmod mymounts.ko

remove:
	- sudo rmmod mymounts.ko 2>/dev/null
