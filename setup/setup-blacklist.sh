#!/bin/bash

if [ "$EUID" -ne 0 ]; then
   echo "This script must be run as root"
   exit 1
fi

echo "blacklist usb_f_ecm" >> /etc/modprobe.d/blacklist.conf
echo "blacklist g_ether" >> /etc/modprobe.d/blacklist.conf
reboot
