# Linux Character Device Driver

This project demonstrates the development of a **basic Linux Character Device Driver** as a loadable kernel module.

The driver allows communication between **user space and kernel space** using standard file operations such as `open`, `read`, `write`, and `release`.

---

## Features

- Dynamic device number allocation
- Character device registration using `cdev`
- Implementation of `file_operations`
- User-kernel communication using:
  - `copy_to_user()`
  - `copy_from_user()`
- Kernel logging using `printk()`

---

## Project Files
mychardev.c → Character device driver source code
Makefile → Kernel module build configuration


---

## Build the Driver

Compile the kernel module using the kernel build system:

```bash
make
This generates the module:

mychardev.ko
Load the Driver

Insert the kernel module:

sudo insmod mychardev.ko

Check kernel logs:

sudo dmesg | tail
Create Device Node

After loading the driver, create the device file:

sudo mknod /dev/mychardev c <major_number> 0
sudo chmod 666 /dev/mychardev

You can get the major number from:

sudo dmesg | tail

Test the Driver

Write data to the device:

echo "Hello Driver" > /dev/mychardev

Read data from the device:

cat /dev/mychardev

Check kernel logs:

sudo dmesg | tail
