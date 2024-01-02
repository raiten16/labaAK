
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include <linux/errno.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>

MODULE_AUTHOR("Jasinsky Lubomur <raiten16@gmail.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int repeat_count = 1;
module_param(repeat_count, uint, 0644);
MODULE_PARM_DESC(repeat_count, "Скільки разів вивести рядок 'Hello, world!'");

struct event_data {
	struct list_head list;
	ktime_t timestamp;
};

static LIST_HEAD(event_list);


static int __init hello_init(void)
{
	if (repeat_count == 0 || (repeat_count >= 5 && repeat_count <= 10)) {
		pr_warn("Невірне значення параметра.\n");
	} else if (repeat_count > 10) {
		pr_err("Помилкове значення параметра. Модуль не завантажено.\n");
	return -EINVAL;
}

	struct event_data *event = kmalloc(sizeof(*event), GFP_KERNEL);

	event->timestamp = ktime_get();
	list_add(&event->list, &event_list);

	int i;

	for (i = 0; i < repeat_count; i++)
		printk(KERN_EMERG "Hello, world!\n");

	return 0;
}

static void __exit hello_exit(void)
{
	struct event_data *event, *temp;

	list_for_each_entry_safe(event, temp, &event_list, list) {
		ktime_t duration = ktime_sub(ktime_get(), event->timestamp);

		pr_info("Час події: %lld нс\n",
ktime_to_ns(duration));
		list_del(&event->list);
		kfree(event);
}
}

module_init(hello_init);
module_exit(hello_exit);
