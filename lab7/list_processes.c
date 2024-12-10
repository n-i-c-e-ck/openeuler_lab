#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A module to list all process names");
MODULE_VERSION("1.0");

// 模块加载函数
static int __init list_processes_init(void) {
    struct task_struct *task;

    printk(KERN_INFO "Loading Process List Module\n");
    printk(KERN_INFO "Process Names:\n");

    for_each_process(task) {
        // 打印任务名称
        printk(KERN_INFO "Process: %s\n", task->comm);
    }

    return 0;
}

// 模块卸载函数
static void __exit list_processes_exit(void) {
    printk(KERN_INFO "Unloading Process List Module\n");
}

module_init(list_processes_init);
module_exit(list_processes_exit);
