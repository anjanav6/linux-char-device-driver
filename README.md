# Linux Character Device Driver

A **loadable kernel module** that implements a basic Linux character device driver, enabling communication between **user space** and **kernel space** via standard file operations.

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Project Structure](#project-structure)
- [Prerequisites](#prerequisites)
- [Build](#build)
- [Usage](#usage)
  - [Load the Driver](#load-the-driver)
  - [Create Device Node](#create-device-node)
  - [Test the Driver](#test-the-driver)
  - [Unload the Driver](#unload-the-driver)
- [Kernel Logs](#kernel-logs)
- [How It Works](#how-it-works)
- [License](#license)

---

## Overview

This project demonstrates the fundamentals of Linux kernel module development. The driver registers a character device that user-space programs can interact with using standard POSIX file operations — just like reading and writing to a regular file.

---

## Features

- Dynamic device number allocation via `alloc_chrdev_region()`
- Character device registration using the `cdev` interface
- Full `file_operations` implementation (`open`, `read`, `write`, `release`)
- Safe user–kernel data transfer using `copy_to_user()` and `copy_from_user()`
- Kernel-space logging via `printk()`

---

## Project Structure

```
.
├── mychardev.c     # Character device driver source code
└── Makefile        # Kernel module build configuration
```

---

## Prerequisites

- Linux kernel headers for your running kernel:
  ```bash
  sudo apt install linux-headers-$(uname -r)   # Debian/Ubuntu
  sudo dnf install kernel-devel                 # Fedora/RHEL
  ```
- `gcc`, `make`, and standard build tools

---

## Build

Compile the kernel module using the kernel build system:

```bash
make
```

This generates the loadable module:

```
mychardev.ko
```

To clean build artifacts:

```bash
make clean
```

---

## Usage

### Load the Driver

Insert the kernel module:

```bash
sudo insmod mychardev.ko
```

Verify it loaded successfully:

```bash
lsmod | grep mychardev
```

### Create Device Node

Retrieve the dynamically assigned major number from the kernel log:

```bash
sudo dmesg | tail
```

Then create the device file and set permissions:

```bash
sudo mknod /dev/mychardev c <major_number> 0
sudo chmod 666 /dev/mychardev
```

> Replace `<major_number>` with the value printed in `dmesg`.

### Test the Driver

**Write data to the device:**

```bash
echo "Hello Driver" > /dev/mychardev
```

**Read data back from the device:**

```bash
cat /dev/mychardev
```

**Check kernel logs to observe driver activity:**

```bash
sudo dmesg | tail
```

### Unload the Driver

Remove the module when done:

```bash
sudo rmmod mychardev
```

---

## Kernel Logs

All driver events (open, read, write, release) are logged via `printk()`. Monitor them in real time with:

```bash
sudo dmesg -w
```

Or view the last few entries:

```bash
sudo dmesg | tail -20
```

---

## How It Works

| Step | Description |
|------|-------------|
| `insmod` | Kernel calls the module's `init` function; a device number is allocated and the `cdev` is registered |
| `open` | User opens `/dev/mychardev`; the driver's `open` handler is invoked |
| `write` | Data flows from user space → kernel buffer via `copy_from_user()` |
| `read` | Data flows from kernel buffer → user space via `copy_to_user()` |
| `release` | File descriptor is closed; driver's `release` handler cleans up |
| `rmmod` | Kernel calls the module's `exit` function; device is unregistered and memory freed |

---

## License

This project is intended for educational purposes. See [LICENSE](LICENSE) for details.
