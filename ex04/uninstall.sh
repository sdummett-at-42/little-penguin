#!/bin/sh

MODNAME="hello-world.ko"

echo "[+] Removing /tmp/$MODNAME"
sudo rm /tmp/$MODNAME

echo "[+] Removing the udev rule /etc/udev/rules.d/99-usb-kbd.rules"
sudo rm /etc/udev/rules.d/99-usb-kbd.rules

echo "[+] Reloading the udev rules"
sudo udevadm control --reload
