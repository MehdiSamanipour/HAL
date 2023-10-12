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

struct gpio_dev {
    int no; // gpio number
    int dir; // 0 = in, 1 = out
    char *name;
    
};

static struct gpio_dev gpio_devs[] = {
    {12, 0, "BTN0"},
    {16, 0, "BTN1"},
    {20, 1, "LED1"},
    {21, 1, "LED3"},
    {26, 1, "LED0"}
};

static int gpios_len = 5;

// Declare the probe and remove functions for the platform device
static int LED3_probe(struct platform_device *pdev)
{
    printk(KERN_INFO "hello from probe\n");
    int err = 0;
    for (int i = 0; i < gpios_len; i++) {
        err = gpio_request(gpio_devs[i].no, gpio_devs[i].name);
        if (err != 0) {
            printk("gpio request failed");
            goto fail_gpio_request;
        }
    }

    for (size_t di = 0; di < gpios_len; di++) {
        if (gpio_devs[di].dir) {
            err = gpio_direction_output(gpio_devs[di].no, 0);
        } else {
            err = gpio_direction_input(gpio_devs[di].no);
        }
        if (err != 0) {
            printk("gpio direction failed");
            goto fail_gpio_direction;
        } 
    }

    char buf[7];
    for (size_t cr = 0; cr < gpios_len; cr++) {
        err = device_create(LED3_class, NULL, MKDEV(MAJOR(devno), cr), NULL, "%s", gpio_devs[cr].name);
        if (err != 0) {
            goto fail_device_create;
        }
    }

    return 0;

fail_device_create:
    for (size_t cr = 0; cr < gpios_len; cr++) {
        device_destroy(LED3_class, MKDEV(MAJOR(devno), cr));
    }

fail_gpio_direction:
    for (size_t i = 0; i < gpios_len; i++) {
        gpio_free(gpio_devs[i].no);
    }

fail_gpio_request:
    return -1;

    
 }

static int LED3_remove(struct platform_device *pdev)
{
    printk(KERN_INFO "hello from remove\n");
    for (size_t cr = 0; cr < gpios_len; cr++) {
        device_destroy(LED3_class, MKDEV(MAJOR(devno), cr));
        gpio_free(gpio_devs[cr].no);
    }
    return 0;
}

static const struct of_device_id LED3_of_match[] = {
    { .compatible = "ase,led3" },
    {},
};
// Define the platform driver structure
static struct platform_driver LED3_driver = {
.driver = {
.name = DRIVER_NAME,
.of_match_table = LED3_of_match,
.owner = THIS_MODULE,
},
.probe = LED3_probe,
.remove = LED3_remove,
};

// Initialize the module
static int __init LED3_init(void)
{
int ret;
printk(KERN_INFO "LED3 driver init\n");

// Allocate a range of character device numbers
ret = alloc_chrdev_region(&devno, first_minor, max_devices, DEVICE_NAME);
if (ret < 0) {
    printk(KERN_ALERT "Failed to allocate device number for LED3 driver\n");
    return ret;
}

// Create the LED3 class
LED3_class = class_create(THIS_MODULE, CLASS_NAME);
if (IS_ERR(LED3_class)) {
    unregister_chrdev_region(devno, max_devices);
    printk(KERN_ALERT "Failed to create class for LED3 driver\n");
    return PTR_ERR(LED3_class);
}

// Register the platform driver
ret = platform_driver_register(&LED3_driver);
if (ret != 0) {
    class_destroy(LED3_class);
    unregister_chrdev_region(devno, max_devices);
    printk(KERN_ALERT "Failed to register platform driver for LED3\n");
    return ret;
}

printk(KERN_INFO "LED3 driver initialized successfully\n");
return 0;

}

// Exit the module
static void __exit LED3_exit(void)
{
printk(KERN_INFO "LED3 driver exit\n");


// Unregister the platform driver
platform_driver_unregister(&LED3_driver);

// Destroy the LED3 class
class_destroy(LED3_class);

// Free the character device number range
unregister_chrdev_region(devno, max_devices);
}

module_init(LED3_init);
module_exit(LED3_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HAL_2023");