# PineDucky

This project aims to convert your Pinephone into a Rubber Ducky (hid gadget).

- Required Pinephone kernel >= 3.19

This project is not finished, so a couple things are not implemented yet. Todos written in **TODO.md** :)

### Tested Pinephone OS's
- Mobian (Linux 5.10-sunxi64 aarch64 GNU/Linux)

### Setup

```
git clone https://github.com/Sherknob/PineDucky.git
cd PineDucky
make
```
**Please** have a look into **setup/README.md** for further instructions on your setup.

After you have done the steps in **setup/README.md**, you can do a test run. Please look into **testing/README.md** for further instructions.

**Sources:**
[Linux USB gadget configured through configfs](https://www.kernel.org/doc/html/latest/usb/gadget_configfs.html)  
[Configfs - Userspace-driven Kernel Object Configuration](https://www.kernel.org/doc/html/latest/filesystems/configfs.html)  
[Using RPi Zero as a Keyboard](https://www.rmedgar.com/blog/using-rpi-zero-as-keyboard-setup-and-device-definition/)  
[ersal Serial Bus HID Usage Tables](https://d1.amobbs.com/bbs_upload782111/files_47/ourdev_692986N5FAHU.pdf)  
[Kernel modules](https://wiki.gentoo.org/wiki/Kernel_Modules)
[How to blacklist a module on Ubuntu/Debian Linux](https://linuxconfig.org/how-to-blacklist-a-module-on-ubuntu-debian-linux)  
[Translation to German keyboard layout](https://stackoverflow.com/questions/9777286/sending-the-right-hid-keycode)  
[USB HID Keyboard scan codes](https://gist.github.com/MightyPork/6da26e382a7ad91b5496ee55fdc73db2)
