#!/bin/sh

echo "[+] Compiling the module"
make

MODNAME="hello-world.ko"

echo "[+] Copying $MODNAME in /tmp"
sudo cp $MODNAME /tmp

echo "[+] Installing the new udev rule in /etc/udev/rules.d/"
sudo cp ./99-usb-kbd.rules /etc/udev/rules.d/

echo "[+] Reloading the udev rules"
sudo udevadm control --reload

echo ""
echo "[+] Now to test if the module is properly working, you need to add a usb keyboard."
echo "[+] It can be done in the qemu monitor interface (CTRL+ALT+2) by submitting the command:"
echo "[+] device_add usb-kbd,id=USB42"
echo "[+] To remove the device, you can do:"
echo "[+] device_del USB42"
