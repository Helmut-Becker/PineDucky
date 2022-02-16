## Setup

To make the Pinephone appear as a keyboard, you need to enable the `configfs` kernel module. That requires you to build your own kernel, if this is not supported right out of the Box.
To check if configfs is available as a module or even build into the kernel execute:
```
sudo lsmod | grep configfs
cat /lib/modules/$(uname -r)/modules.builtin
find /lib/modules/$(uname -r) -type f -name '*.ko*'
```
If you can find configfs in there, you can (if not already loaded or even built in) load it with `sudo modprobe configfs`.

#### Blacklist interfering kernel modules

Sometimes the activation of a gadget can throw an error like `echo: write error: Device or resource busy` when trying to write into `UDC`. Because some other gadget is in place. One option to disable running gadgets is to blacklist them and prevent them from starting at all.

To find them do:
```
lsmod | grep _f_
```
For me on Mobian (Linux 5.10-sunxi64 aarch64 GNU/Linux) disabling `usb_f_ecm` and `g_ether` did the trick.
**Note:** If you are on Mobian, you can just execute:
```
sudo sh setup-blacklist.sh
```

You can blacklist modules in `/etc/modprobe.d/blacklist.conf` with `blacklist <module>` in my case `blacklist g_ether` and `blacklist usb_f_ecm`.

After you blacklisted modules, you need to update your initramfs in order to apply changes. Execute:
```
sudo update-initramfs -u
sudo reboot
```

#### Starting gadget

To start the hid gadget on the Pinephone do:
```
sudo sh start-gadget.sh
```
**Note:** Do this without anything plugged into your Pinephone.

#### Writing to /dev/hidg0

If everything worked as expected you can now write to `/dev/hidg0`.
On how and what to write to `/dev/hidg0`, please look into the **Sources:** at the end of the main **README.md** file.
