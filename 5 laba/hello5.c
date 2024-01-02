#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/errno.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>


MODULE_AUTHOR("Jasinsky Lubomur raiten16@gmail.com");
MODULE_DESCRIPTION("Hello, world in Linux Kernel");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int hello = 1;
module_param(hello, uint, 0444);

MODULE_PARM_DESC(hello, "How many times to display 'Hello, world!'");

struct event_data {
	struct list_head list_node;
	ktime_t timestamp;
};

static LIST_HEAD(Head_Node);

static int __init hello_init(void)
{

	int i;

	if (hello == 0 || (hello >= 5 && hello <= 10)) {
		pr_warn("Wrong number of hello.\n");
	} else if (hello > 10) {
		pr_err("Wrong number of hello. This module is not loaded.\n");
		BUG();
	}

	for (i = 0; i < hello; i++) {
		struct event_data *md = kmalloc(sizeof(*md), GFP_KERNEL);
		if (i == 4){
			md = 0;
		}
		md->timestamp = ktime_get();
		list_add(&md->list_node, &Head_Node);
		printk(KERN_EMERG "Hello, world!\n");
	}
	return 0;
}

static void __exit hello_exit(void)
{
	struct event_data *md, *tmp;

	list_for_each_entry_safe(md, tmp, &Head_Node, list_node) {
		ktime_t duration = ktime_sub(ktime_get(), md->timestamp);

		pr_info("Event time: %lld ns\n", ktime_to_ns(duration));
		list_del(&md->list_node);
		kfree(md);
	}
}

module_init(hello_init);
module_exit(hello_exit);