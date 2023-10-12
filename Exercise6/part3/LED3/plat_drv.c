#include <linux/err.h>
#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
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

/* GPIO Device Data Struct*/
struct gpio_dev {
    int gpio;
    int flag;
};

static struct gpio_dev gpio_devs[255];
int gpio_devs_cnt = 0;
int gpios_in_dt;


static int LED3_probe(struct platform_device *pdev)
{
    int err = 0;
    struct device *dev = &pdev->dev; // Device ptr
    struct device_node *np = dev->of_node; // Device tree node ptr
    enum of_gpio_flags flag;
    int gpios_in_dt=0;

    /* Retrieve number of GPIOs */
    gpios_in_dt = of_gpio_count(np);

    /* Loop through gpios in Device Tree */
    for (int i = 0; i < gpios_in_dt; i++) {
        /* Get gpio number and write to struct */
        gpio_devs[gpio_devs_cnt].gpio = of_get_gpio(np, i);

        /* Get gpio flag (direction) and write to struct */
        flag = of_get_gpio_flags(np, i,NULL);
        if (flag == GPIOF_ACTIVE_LOW) {
            gpio_devs[gpio_devs_cnt].flag = GPIOF_OUT_INIT_HIGH;
        } else {
            gpio_devs[gpio_devs_cnt].flag = GPIOF_OUT_INIT_LOW;
        }

        gpio_devs_cnt++; // Increment gpio_devs counter
    }

    for (int i = 0; i < gpios_in_dt; i++) {
        /* Request GPIO */
        err = gpio_request(gpio_devs[i].gpio, "LED3");
        if (err != 0) {
            printk(KERN_ALERT "GPIO request failed\n");
            goto fail_gpio_request;
        }

        /* Set GPIO direction */
        if (gpio_direction_output(gpio_devs[i].gpio, 0) != 0) {
            printk(KERN_ALERT "GPIO direction failed\n");
            goto fail_gpio_direction;
        }

        /* Create devices */
        if (device_create(LED3_class, NULL, MKDEV(MAJOR(devno), i), NULL, "LED3_%d", i) == NULL) {
            printk(KERN_ALERT "Device creation failed\n");
            goto fail_device_create;
        }

        printk(KERN_ALERT "Using GPIO[%i], flag:%i on major:%i, minor:%i\n",
            gpio_devs[i].gpio, gpio_devs[i].flag,
            MAJOR(devno), i);
    }

    return 0;

fail_device_create:

    for (int j = 0; j < gpio_devs_cnt; j++) {
        device_destroy(LED3_class, MKDEV(MAJOR(devno), j));
    }

fail_gpio_direction:

    for (int j = 0; j < gpio_devs_cnt; j++) {
        gpio_free(gpio_devs[j].gpio);
    }


fail_gpio_request:

    gpio_devs_cnt = 0;

fail_gpio_count:

    return err;
}

static int LED3_remove(struct platform_device *pdev)
{
    for (int i = 0; i < gpios_in_dt; i++) {
        device_destroy(LED3_class, MKDEV(MAJOR(devno), i));
        gpio_free(gpio_devs[i].gpio);
    }

    gpio_devs_cnt = 0;

    return 0;
}

static const struct of_device_id LED3_dt_ids[] = {
    { .compatible = "ase,led3", },
    {},
};

MODULE_DEVICE_TABLE(of, LED3_dt_ids);

static struct platform_driver LED3_platform_driver = {
    .probe = LED3_probe,
    .remove = LED3_remove,
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = LED3_dt_ids,
        .owner = THIS_MODULE,
    },
};

static int __init LED3_init(void)
{
    int err = 0;

    // Allocate device numbers
    err = alloc_chrdev_region(&devno, first_minor, max_devices, DRIVER_NAME);
    if (err < 0) {
        printk(KERN_ALERT "Device number allocation failed\n");
        return err;
    }

    // Create class
    LED3_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(LED3_class)) {
        printk(KERN_ALERT "Class creation failed\n");
        unregister_chrdev_region(devno, max_devices);
        return PTR_ERR(LED3_class);
    }

    // Register platform driver
    err = platform_driver_register(&LED3_platform_driver);
    if (err < 0) {
        printk(KERN_ALERT "Platform driver registration failed\n");
        class_destroy(LED3_class);
        unregister_chrdev_region(devno, max_devices);
        return err;
    }

    printk(KERN_ALERT " :-) platform driver loaded :-)\n");
    return 0;
}

static void __exit LED3_exit(void)
{
    // Unregister platform driver
    platform_driver_unregister(&LED3_platform_driver);

    // Destroy class
    class_destroy(LED3_class);

    // Release device numbers
    unregister_chrdev_region(devno, max_devices);

    printk(KERN_ALERT "LED3 platform driver unloaded\n");
}

module_init(LED3_init);
module_exit(LED3_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HAL_2023");
MODULE_DESCRIPTION("Platform Driver for LED3");

