# Linux char device driver basics
This is a simple driver and userspace app that communicates with the driver through ioctl. It has two different commands, one prints the program accessing the device to the kernel log buffer the other is just an argument passing test.

## Building
```
make && make prog
```

## Running
Use insmod to enable the module:
```
sudo insmod customdevice.ko
```

use the program to execute commands and then check `dmesg` for output.
```
sudo ./userspace
```

## Details
This driver and program are built using gcc 10.2. The driver is built against the 5.11.16 kernel source.
