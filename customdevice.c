#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/sched.h>
#include <asm/uaccess.h>

struct _customdevice_data {
	struct cdev cdev;
	uint8_t data;
};
typedef struct _customdevice_data cd_data;

static cd_data customdevice_data;
static struct class *cl;
static dev_t dev;


static int customdevice_open(struct inode *inode, struct file *file) {
	// cd_data *customdevice_data = container_of(inode->i_cdev, cd_data, cdev);
	return 0;
}

static int customdevice_release(struct inode *inode, struct file *file) {
	// cd_data *customdevice_data = container_of(inode->i_cdev, cd_data, cdev);
	return 0;
}

static long customdevice_ioctl(struct file *file,
			       unsigned int cmd,
			       unsigned long arg) {
	switch(cmd) {
	case 0xFFAB:
		printk(KERN_INFO "Custom device: %d, %s",
		       task_pid_nr(current),
		       current->comm);
		break;
	case 0xFFAC:
		if (arg == 8) {
		printk(KERN_INFO "Argument is: %ld",
		       arg);
		} else {
			printk(KERN_WARNING "Wrong argument");
			return -1;
		}
		break;
	}
	return 0;
}

const struct file_operations customdevice_fops = {
    .owner = THIS_MODULE,
    .open = customdevice_open,
    .release = customdevice_release,
    .unlocked_ioctl = customdevice_ioctl
};

static int __init customdevice_init(void) {
	int ret;
	struct device *dev_ret;

	// Create character device region
	ret = alloc_chrdev_region(&dev, 0, 1, "customdevice");
	if (ret < 0) {
		return ret;
	}

	// Create class for sysfs
	cl = class_create(THIS_MODULE, "chardrv");
	if (IS_ERR(cl)) {
		unregister_chrdev_region(dev, 1);
		return PTR_ERR(cl);
	}

	// Create device for sysfs
	dev_ret = device_create(cl, NULL, dev, NULL, "customdevice");
	if (IS_ERR(dev_ret)) {
		class_destroy(cl);
		unregister_chrdev_region(dev, 1);
		return PTR_ERR(dev_ret);
	}

	// Create character device
	cdev_init(&customdevice_data.cdev, &customdevice_fops);
	ret = cdev_add(&customdevice_data.cdev, dev, 1);
	if (ret < 0) {
		device_destroy(cl, dev);
		class_destroy(cl);
		unregister_chrdev_region(dev, 1);
		return ret;
	}

	printk(KERN_INFO "Custom device initialized");
	return 0;
}

static void __exit customdevice_exit(void) {
	device_destroy(cl, dev);
	class_destroy(cl);
	cdev_del(&customdevice_data.cdev);
	unregister_chrdev_region(dev, 1);
}

module_init(customdevice_init);
module_exit(customdevice_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tristan Šneider <tristan.sneider@gmail.com>");
MODULE_DESCRIPTION("Custom char device driver");
