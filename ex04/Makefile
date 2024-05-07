obj-m += usb-kbd.o

PWD := $(CURDIR)

MODNAME="usb-kbd.ko"

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean%

install: remove
	@echo "[+] Copying $(MODNAME) in /tmp"
	@sudo cp $(MODNAME) /tmp

	@echo "[+] Installing the new udev rule in /etc/udev/rules.d/"
	@sudo cp ./99-usb-kbd.rules /etc/udev/rules.d/

	@echo "[+] Reloading the udev rules"
	@sudo udevadm control --reload

	@echo "[+] Inserting the module"
	@sudo insmod $(MODNAME)

	@echo ""
	@echo "[+] Now to test if the module is properly working, you need to add a usb keyboard."
	@echo "[+] It can be done in the qemu monitor interface (CTRL+ALT+2) by submitting the command:"
	@echo "[+] device_add usb-kbd,id=USB42"
	@echo "[+] To remove the device, you can do:"
	@echo "[+] device_del USB42"

remove:
	@echo "[+] Removing /tmp/$(MODNAME)"
	- @sudo rm /tmp/$(MODNAME)

	@echo "[+] Removing the udev rule /etc/udev/rules.d/99-usb-kbd.rules"
	- @sudo rm /etc/udev/rules.d/99-usb-kbd.rules

	@echo "[+] Reloading the udev rules"
	- @sudo udevadm control --reload

	@echo "[+] Removing the module"
	- @sudo rmmod $(MODNAME)
