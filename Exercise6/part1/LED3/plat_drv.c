#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#define DEVICE_NAME "LED3"
#define DRIVER_NAME "LED3_driver"
#define CLASS_NAME "LED3_class"
static dev_t devno;
static struct class *LED3_class;
const int first_minor = 0;
unsigned gpio = 21;
const int max_devices = 255;
static struct cdev LED3_cdev;
struct device *LED3_device = NULL;
static struct device *dev;
// Declare the probe and remove functions for the platform device
static int LED3_probe(struct platform_device *pdev)
{
    printk(KERN_INFO "hello from probe\n");

 LED3_device = device_create(LED3_class, NULL, MKDEV(MAJOR(devno), first_minor), NULL, "LED3_%d", gpio);
    return 0;
}
static int LED3_remove(struct platform_device *pdev)
{
    printk(KERN_INFO "hello from remove\n");
    device_destroy(LED3_class, MKDEV(MAJOR(devno), first_minor));
    gpio_free(gpio);
    return 0;
}
// Define the compatible device name for the platform driver and create a table of device IDs
static const struct of_device_id LED3_of_match[] = {
    { .compatible = "ase,LED3", },  // Compatible device name
    {},
};
MODULE_DEVICE_TABLE(of, LED3_of_match);
// Declare and initialize the platform driver struct with the probe and remove functions
static struct platform_driver LED3_platform_driver = {
    .driver = {
        .name = DRIVER_NAME,  // Driver name
        .owner = THIS_MODULE,
        .of_match_table = LED3_of_match,  // Table of compatible device IDs
    },
    .probe = LED3_probe,
    .remove = LED3_remove,
};



// Define the initialization and exit functions for the module
static int __init LED3_init(void)
{
    int ret;
    // Allocate a device number
    ret = alloc_chrdev_region(&devno, 0, 1, DEVICE_NAME);
    if (ret < 0) {
        printk(KERN_ERR "Failed to allocate device number\n");
        return ret;
    }
    // Create a device class
    LED3_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(LED3_class)) {
        unregister_chrdev_region(devno, 1);
        printk(KERN_ERR "Failed to create device class\n");
        return PTR_ERR(LED3_class);
    }
    // Create a device
    dev = device_create(LED3_class, NULL, devno, NULL, DEVICE_NAME);
    if (IS_ERR(dev)) {
        class_destroy(LED3_class);
        unregister_chrdev_region(devno, 1);
        printk(KERN_ERR "Failed to create device\n");
        return PTR_ERR(dev);
    }
    // Register the platform driver
    ret = platform_driver_register(&LED3_platform_driver);
    if (ret < 0) {
        device_destroy(LED3_class, devno);
        class_destroy(LED3_class);
        unregister_chrdev_region(devno, 1);
        printk(KERN_ERR "Failed to register platform driver\n");
        return ret;
    }
    printk(KERN_INFO "LED3 registered\n");
    return 0;
}
static void __exit LED3_exit(void)
{
    // Unregister the platform driver
    platform_driver_unregister(&LED3_platform_driver);
    // Destroy the device
    device_destroy(LED3_class, devno);
    // Destroy the class
    class_destroy(LED3_class);
    // Release the device number
    unregister_chrdev_region(devno, 1);
    printk(KERN_INFO "LED3 unregistered\n");
}
int LED3_open(struct inode *inode, struct file *filep);
int LED3_release(struct inode *inode, struct file *filep);
ssize_t LED3_read(struct file *filep, char __user *buf, size_t count, loff_t *f_pos);
ssize_t LED3_write(struct file *filep, const char __user *ubuf, size_t count, loff_t *f_pos);
struct file_operations LED3_fops = {
    .owner = THIS_MODULE,
    .open = LED3_open,
    .release = LED3_release,
    .read = LED3_read,
    .write = LED3_write,
};
static int _init(void)
{
   // Request GPIO
   int err = gpio_request(gpio, "LED3");
   // Set GPIO direction (in or out)
   err = gpio_direction_output(gpio,0);
   // Alloker Major/Minor
   err = alloc_chrdev_region(&devno, first_minor, max_devices, "LED3");
   if (MAJOR(devno) <= 0)
      pr_err("Faild to register chardev\n");
   pr_info("LED3 driver got Major %i\n", MAJOR(devno));
   // Class Create
   LED3_class = class_create(THIS_MODULE, "LED3");
   if (IS_ERR(LED3_class))
      pr_err("Failed to create class");
   // Cdev Init
   cdev_init(&LED3_cdev, &LED3_fops);
   // Add Cdev
   err = cdev_add(&LED3_cdev, devno, max_devices);
   if (err)
      pr_err("Failed to add cdev");
   return err;
}
static void _exit(void)
{
   // Delete Cdev
   cdev_del(&LED3_cdev);
   // Unregister Device
   unregister_chrdev_region(devno, max_devices);
   // Class Destroy
   class_destroy(LED3_class);
   // Free GPIO
   gpio_free(gpio);
}

int LED3_open(struct inode *inode, struct file *filep)
{
   int major, minor;
   major = MAJOR(inode->i_rdev);
   minor = MINOR(inode->i_rdev);
   printk("Opening MyGpio Device [major], [minor]: %i, %i\n", major, minor);
   return 0;
}
int LED3_release(struct inode *inode, struct file *filep)
{
   int minor, major;

   major = MAJOR(inode->i_rdev);
   minor = MINOR(inode->i_rdev);
   printk("Closing/Releasing MyGpio Device [major], [minor]: %i, %i\n", major, minor);

   return 0;
}
ssize_t LED3_read(struct file *filep, char __user *buf, size_t count, loff_t *f_pos)
{
   char kbuf[2];
   int len, value;
   value = gpio_get_value(gpio);
   len = count < 2 ? count : 2;            /* Truncate to smallest */
   len = snprintf(kbuf, len, "%i", value); /* Create string */
   value = copy_to_user(buf, kbuf, ++len); /* Copy to user */
   *f_pos += len;                          /* Update cursor in file */
   return len;
} /* Return length read */
ssize_t LED3_write(struct file *filep, const char __user *ubuf, size_t count, loff_t *f_pos)
{
   char kbuf[12];
   int len, value, err = 0;
   len = count < 12 ? count : 2; /* Truncate to smaller buffer size */
   err = copy_from_user(kbuf, ubuf, len);
   if (err < 0)
      return err;
   kbuf[len] = 0;
   err = kstrtoint(kbuf, 0, &value);
   /* Convert sting to int */
   if (err)
      pr_err("Error converting string to int\n");
   gpio_set_value(gpio, value);
   pr_info("Wrote %i from minor %i\n", value, iminor(filep->f_inode));
   *f_pos += len; /* Update cursor in file */
   return len;    /* return length actually written */
   // Hvilke trin er det der skal udføres?
   // Hint konvertering fra string til int kan gøres via sscanf() - antagelsen er at det er strenge der sendes til og fra user space.
   return count;
}
module_init(LED3_init);
module_exit(LED3_exit);
MODULE_AUTHOR("HAL_2023");
MODULE_LICENSE("GPL");

