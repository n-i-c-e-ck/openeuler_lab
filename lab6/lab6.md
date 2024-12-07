#### 打印当前任务用户态和内核态堆栈起始地址模块

---

#### 1. 模块概述

本内核模块旨在打印当前任务的用户态和内核态堆栈起始地址。通过读取当前任务的 `task_struct` 结构，该模块可以获取并输出用户态堆栈起始地址（如果存在）以及内核态堆栈起始地址。这有助于开发者和系统管理员了解当前任务的堆栈信息。

#### 2. 技术要求

- **熟悉 Linux 内核结构**：了解 `task_struct` 和 `mm_struct` 等内核数据结构。
- **堆栈管理**：理解 Linux 内核中的用户态和内核态堆栈的概念。
- **内核模块编程**：掌握基本的内核模块编写和调试技巧。

#### 3. 参考资源

- [Linux 内核文档](https://www.kernel.org/doc/html/latest/)
- [Linux 内核源码](https://elixir.bootlin.com/linux/latest/source/)

#### 4. 使用方法

##### 4.1 编译

确保你已经安装了内核开发工具链，并且有权限访问内核源代码。使用以下命令编译模块：

```sh
make
```

##### 4.2 加载模块

编译完成后，可以通过以下命令加载模块：

```sh
sudo insmod stack_print.ko
```

##### 4.3 查看输出

模块加载后，你可以通过 `dmesg` 命令查看输出信息：

```sh
dmesg | tail
```

预期输出示例：

```plaintext
[   60.123456] 用户态堆栈起始: 0x7fff0000
[   60.123457] 内核态堆栈起始: 0xffff8800
```

如果当前任务是内核线程，则用户态堆栈信息会显示为不可用：

```plaintext
[   60.123456] 用户态堆栈: 不可用 (内核线程)
[   60.123457] 内核态堆栈起始: 0xffff8800
```

##### 4.4 卸载模块

完成测试后，可以通过以下命令卸载模块：

```sh
sudo rmmod stack_print
```

模块卸载时会打印一条消息确认卸载完成：

```plaintext
[   65.123456] 模块卸载完成
```

#### 5. 代码解析

##### 5.1 包含头文件

```c
#include <linux/module.h>    // 内核模块支持
#include <linux/kernel.h>    // 内核空间函数和宏定义
#include <linux/sched/task.h>// 任务调度结构体定义
```

##### 5.2 初始化函数

```c
static int print_stack_addrs(void) {
    struct task_struct *task = current;
    unsigned long user_stack_start = 0;
    unsigned long kernel_stack_start = 0;

    if (task->mm) {
        user_stack_start = task->mm->start_stack;
        pr_info("用户态堆栈起始: 0x%lx\n", user_stack_start);
    } else {
        pr_info("用户态堆栈: 不可用 (内核线程)\n");
    }

    kernel_stack_start = (unsigned long)task->stack;
    pr_info("内核态堆栈起始: 0x%lx\n", kernel_stack_start);

    return 0;
}
```

- **获取当前任务的 `task_struct` 和 `mm_struct`**：使用 `current` 宏获取当前任务的任务结构指针，并从中提取内存管理结构指针。
- **遍历 PGD 条目**：检查是否存在 `mm` 结构以确定是否为用户进程，并打印相应的用户态堆栈起始地址或提示信息。
- **获取内核态堆栈起始地址**：直接从 `task->stack` 获取内核态堆栈起始地址并打印。

##### 5.3 清理函数

```c
static void cleanup(void) {
    pr_info("模块卸载完成\n");
}
```

- **打印卸载信息**：在模块卸载时打印一条确认消息。

##### 5.4 模块宏定义

```c
module_init(print_stack_addrs);
module_exit(cleanup);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("打印当前任务的用户态和内核态堆栈起始地址");
MODULE_AUTHOR("你的名字");
```

- **指定初始化和清理函数**：使用 `module_init` 和 `module_exit` 宏来指定模块的初始化和清理函数。
- **模块元数据**：定义模块的许可证、描述等信息。

