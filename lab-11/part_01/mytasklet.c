#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <asm/io.h>

#include "ascii.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kovel Alexander");

struct tasklet_struct *my_tasklet;
char my_tasklet_data[] = "key is pressed";

int keyboard_irq = 1;

void my_tasklet_func(unsigned long data)
{
    printk(KERN_INFO "mytasklet: time = %lu", ktime_get());
    printk(KERN_INFO "mytasklet: tasklet state = %lu", my_tasklet->state);
    printk(KERN_INFO "mytasklet: key code is %d", data);
    if (data < 84)
        printk(KERN_INFO "mytasklet: the key is %s", ascii[data]);

}

irqreturn_t my_irq_handler(int irq, void *dev)
{
    int code;

    if (irq == keyboard_irq)
    {
        code = inb(0x60);
        my_tasklet->data = code;
        printk(KERN_INFO "mytasklet: time before scheduling = %lu\n", ktime_get());
        printk(KERN_INFO "mytasklet: tasklet state before scheduling = %lu", my_tasklet->state);
        tasklet_schedule(my_tasklet);
        printk(KERN_INFO "mytasklet: tasklet state after scheduling = %lu",my_tasklet->state);
        printk(KERN_INFO "mytasklet: time after scheduling = %lu",ktime_get());

        return IRQ_HANDLED;
    }

    printk(KERN_INFO "mytasklet: called not by keyboard_irq");

    return IRQ_NONE;
}

static int __init my_tasklet_init(void)
{
    int ret;
    printk(KERN_INFO "mytasklet: init");

    my_tasklet = kmalloc(sizeof(struct tasklet_struct), GFP_KERNEL);

    if (my_tasklet == NULL)
    {
        printk(KERN_ERR "mytasklet: kmalloc error");
        return -1;
    }

    tasklet_init(my_tasklet, my_tasklet_func, (unsigned long)my_tasklet_data);

    ret = request_irq(keyboard_irq, my_irq_handler, IRQF_SHARED,
                      "test_my_irq_handler", (void *) my_irq_handler);

    if (ret)
    {
        printk(KERN_ERR "mytasklet: request_irq error");
        kfree(my_tasklet);
    }
    else
        printk(KERN_ERR "mytasklet: loaded");

    return ret;
}

static void __exit my_tasklet_exit(void)
{
    printk(KERN_INFO "mytasklet: exit");

    synchronize_irq(keyboard_irq);
    free_irq(keyboard_irq, my_irq_handler);

    tasklet_kill(my_tasklet);
    kfree(my_tasklet);
    
    printk(KERN_INFO "mytasklet: unloaded");
}

module_init(my_tasklet_init);
module_exit(my_tasklet_exit);
