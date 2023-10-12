 #include <linux/gpio.h> 
 #include <linux/fs.h>
 #include <linux/cdev.h>
 #include <linux/device.h>
 #include <linux/uaccess.h>
 #include <linux/module.h>
 /* request GPIO, returning 0 or negative errno.
  * non-null labels may be useful for diagnostics.
  */
 int gpio_request(unsigned gpio, const char *label);

 /* release previously-claimed GPIO */
 void gpio_free(unsigned gpio);

 /* set as input or output, returning 0 or negative errno */
 int gpio_direction_input(unsigned gpio);
 int gpio_direction_output(unsigned gpio, int default_value);

 /* GPIO INPUT: return zero or nonzero */
 int gpio_get_value(unsigned gpio);

 /* GPIO OUTPUT */
 void gpio_set_value(unsigned gpio, int value);

  int mygpio_open(struct inode *inode, struct file *filep);
   int mygpio_release(struct inode *inode, struct file *filep);
     ssize_t sw2_read(struct file *filep, char __user *buf, size_t count, loff_t *f_pos);

const int first_minor=0;
const int max_devices=255;  
 static dev_t devno;
 static struct class *sw2_class;
 static struct cdev sw2_cdev;
 struct file_operations sw2_fops={
    .owner= THIS_MODULE,.open = mygpio_open,.release= mygpio_release, .read= sw2_read,
 };
 unsigned gpio=16;
 static int mygpio_init(void)
 { 
 // Request GPIO
 int err=gpio_request(gpio,"sw2");
 // Set GPIO direction (in or out)
 err=gpio_direction_input(gpio);
 // Alloker Major/Minor
 err=alloc_chrdev_region(&devno,first_minor,max_devices,"sw2");
 if (MAJOR(devno)<=0) pr_err("Faild to register chardev\n");
pr_info("sw2 driver got Major %i\n", MAJOR(devno));
  // Class Create 
 sw2_class = class_create(THIS_MODULE,"sw2");
 if (IS_ERR(sw2_class))  pr_err("Failed to create class");
 // Cdev Init
 cdev_init(&sw2_cdev,&sw2_fops);

 // Add Cdev
 err=cdev_add(&sw2_cdev,devno,max_devices);
 if(err) pr_err("Failed to add cdev");
 return err;
 }
 static void mygpio_exit(void)
 {
 // Delete Cdev
 cdev_del(&sw2_cdev);
 // Unregister Device
 unregister_chrdev_region(devno,max_devices);
 // Class Destroy
 class_destroy(sw2_class);
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

 ssize_t sw2_read(struct file *filep, char __user *buf, size_t count, loff_t *f_pos) {
char kbuf[2];
int len, value;
value = gpio_get_value(gpio);
len = count < 2 ? count : 2; /* Truncate to smallest */
len = snprintf(kbuf, len , "%i", value); /* Create string */
value=copy_to_user(buf, kbuf, ++len); /* Copy to user */
*f_pos += len; /* Update cursor in file */
return len; } /* Return length read */
 
module_init(mygpio_init);
module_exit(mygpio_exit);
MODULE_AUTHOR("HAL_2023");
MODULE_LICENSE("GPL");