#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/module.h>

 /* request GPIO, returning 0 or negative errno.
 non-null labels may be useful for diagnostics.
 
int gpio_request(unsigned gpio, const char *label);

release previously-claimed GPIO 
void gpio_free(unsigned gpio);

set as input or output, returning 0 or negative errno 
int gpio_direction_input(unsigned gpio); 
int gpio_direction_output(unsigned gpio, int default_value);

 GPIO INPUT: return zero or nonzero 
int gpio_get_value(unsigned gpio);

GPIO OUTPUT 
void gpio_set_value(unsigned gpio, int value); */


int mygpio_open(struct inode *inode, struct file *filep);
int mygpio_release(struct inode *inode, struct file *filep);
ssize_t mygpio_read(struct file *filep, char __user *buf, size_t count, loff_t *f_pos);
ssize_t LED3_write(struct file *filep, const char __user *ubuf, size_t count, loff_t *f_pos);

const int first_minor = 0;
const int max_devices = 255;
static dev_t devno;
static struct class *LED3_class;
static struct cdev LED3_cdev;
struct file_operations LED3_fops = {
    .owner = THIS_MODULE,
    .open = mygpio_open,
    .release = mygpio_release,
    .read = mygpio_read,
    .write = LED3_write,
};
unsigned gpio = 21;
static int mygpio_init(void)
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
static void mygpio_exit(void)
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

int mygpio_open(struct inode *inode, struct file *filep)
{
   int major, minor;
   major = MAJOR(inode->i_rdev);
   minor = MINOR(inode->i_rdev);
   printk("Opening MyGpio Device [major], [minor]: %i, %i\n", major, minor);
   return 0;
}

int mygpio_release(struct inode *inode, struct file *filep)
{
   int minor, major;

   major = MAJOR(inode->i_rdev);
   minor = MINOR(inode->i_rdev);
   printk("Closing/Releasing MyGpio Device [major], [minor]: %i, %i\n", major, minor);

   return 0;
}

ssize_t mygpio_read(struct file *filep, char __user *buf, size_t count, loff_t *f_pos)
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

module_init(mygpio_init);
module_exit(mygpio_exit);
MODULE_AUTHOR("HAL_2023");
MODULE_LICENSE("GPL");
