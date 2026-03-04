#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#define BUFFER_SIZE 1024

static dev_t dev_num;
static struct cdev my_cdev;

static char device_buffer[BUFFER_SIZE];
static int buffer_size = 0;

/* open */
static int my_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Device opened\n");
    return 0;
}

/* release */
static int my_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Device closed\n");
    return 0;
}

/* write */
static ssize_t my_write(struct file *file, const char __user *buf,
                        size_t len, loff_t *offset)
{
    if (len > BUFFER_SIZE)
        len = BUFFER_SIZE;

    copy_from_user(device_buffer, buf, len);
    buffer_size = len;

    printk(KERN_INFO "Data written to driver\n");

    return len;
}

/* read */
static ssize_t my_read(struct file *file, char __user *buf,
                       size_t len, loff_t *offset)
{
    if (*offset >= buffer_size)
        return 0;

    if (copy_to_user(buf, device_buffer, buffer_size))
        return -EFAULT;

    *offset += buffer_size;

    printk(KERN_INFO "Data read from driver\n");

    return buffer_size;
}

/* file operations */
static struct file_operations fops =
{
    .owner = THIS_MODULE,
    .open = my_open,
    .read = my_read,
    .write = my_write,
    .release = my_release,
};

/* init */
static int __init chardev_init(void)
{
    printk(KERN_INFO "Initializing Character Driver\n");

    alloc_chrdev_region(&dev_num, 0, 1, "mychardev");

    cdev_init(&my_cdev, &fops);

    cdev_add(&my_cdev, dev_num, 1);

    printk(KERN_INFO "Driver registered with major number %d\n", MAJOR(dev_num));

    return 0;
}

/* exit */
static void __exit chardev_exit(void)
{
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_num, 1);

    printk(KERN_INFO "Character Driver Removed\n");
}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Anjana");
MODULE_DESCRIPTION("Character Driver with Read/Write");
