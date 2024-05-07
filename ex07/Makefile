obj-m += fortytwo-debugfs.o

PWD := $(CURDIR)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean%

install: remove
	sudo insmod fortytwo-debugfs.ko

remove:
	- sudo rmmod fortytwo-debugfs.ko 2>/dev/null

ls:
	sudo ls -lh /sys/kernel/debug/fortytwo

chmod:
	sudo chmod +rx /sys/kernel/debug
