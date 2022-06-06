#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

// DEFINE MODULE NAME
#define DEVICE_NAME "hello_world"

// FUNCTION DEFINITIONS
static int dev_open(struct inode*, struct file*);

// GLOBAL VARIABLES
static int major;

// FILE OPERATIONS
static struct file_operations fops = {
   .open = dev_open,
   .read = dev_read,
   .write = dev_write,
   .release = dev_release,
};

// INIT FUNCTION IMPLEMENTATION
static int __init hello_world_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);

    if(major < 0) {
        printk(KERN_ALERT "Hello world module has failed.\n");
        return major;
    }

    printk(KERN_INFO "Hello world module has been loaded!\n");
    return 0;
}

// EXIT FUNCTION IMPLEMENTATION
static void __exit hello_world_exit(void) {
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "Hello world module has been unloaded!\n");
}

// OPEN MODULE
static int dev_open(struct inode *inodep, struct file *filep) {
   printk(KERN_INFO "Rickroll device opened\n");
   return 0;
}

// WRITE TO MODULE (READ_ONLY)
static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset) {

   printk(KERN_INFO "Sorry, rickroll is read only\n");
   return -EFAULT;
}

// CLOSE MODULE
static int dev_release(struct inode *inodep, struct file *filep) {
   printk(KERN_INFO "Rickroll device closed\n");
   return 0;
}

// READ FROM MODULE AND DISPLAY MESSAGE
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
    int errors = 0;
    char *message = "never gonna give you up, never gonna let you down... ";
    int message_len = strlen(message);

    // RETURN ERROR MESSAGE TO BUFFER
    errors = copy_to_user(buffer, message, message_len);

    // RETURN MESSAGE ELSE RETURN EFAULT
    return errors == 0 ? message_len : -EFAULT;
}

module_init(hello_world_init);
module_exit(hello_world_exit);