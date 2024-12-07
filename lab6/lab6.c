#include <linux/module.h>    // 内核模块支持
#include <linux/kernel.h>    // 内核空间函数和宏定义
#include <linux/sched/task.h>// 任务调度结构体定义

static int print_stack_addrs(void) {
    struct task_struct *task = current;        // 获取当前任务结构指针
    unsigned long user_stack_start = 0;        // 用户态堆栈起始地址
    unsigned long kernel_stack_start = 0;      // 内核态堆栈起始地址

    // 如果任务有用户空间内存描述符 (mm)，则获取用户态堆栈起始地址
    if (task->mm) {
        user_stack_start = task->mm->start_stack;
        pr_info("用户态堆栈起始: 0x%lx\n", user_stack_start);
    } else {
        // 如果没有 mm 结构，则说明是内核线程，用户态堆栈不可用
        pr_info("用户态堆栈: 不可用 (内核线程)\n");
    }

    // 获取当前任务的内核态堆栈起始地址
    kernel_stack_start = (unsigned long)task->stack;
    pr_info("内核态堆栈起始: 0x%lx\n", kernel_stack_start);

    return 0;
}

static void cleanup(void) {
    pr_info("模块卸载完成\n");
}

// 指定模块初始化和卸载函数
module_init(print_stack_addrs);
module_exit(cleanup);

// 模块元数据
MODULE_LICENSE("GPL");                         // 模块许可证为 GPL
MODULE_DESCRIPTION("打印当前任务的用户态和内核态堆栈起始地址"); // 模块描述
MODULE_AUTHOR("你的名字");                     // 模块作者
