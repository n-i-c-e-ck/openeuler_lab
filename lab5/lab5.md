#### 当前进程页表目录获取

---

#### 1. 模块概述

本内核模块旨在打印当前进程的页全局目录（PGD）条目信息。通过读取当前进程的 `task_struct` 和 `mm_struct` 结构，该模块可以获取并输出 PGD 中每个非空条目的虚拟地址。这有助于开发者和系统管理员了解当前进程的页表结构。

#### 2. 使用方法

##### 2.1 编译


```sh
make
```

##### 2.2 加载模块

编译完成后，可以通过以下命令加载模块：

```sh
sudo insmod pgd_print.ko
```

##### 2.3 查看输出

模块加载后，你可以通过 `dmesg` 命令查看输出信息：

```sh
dmesg | tail
```

```plaintext
[   60.123456] 当前进程页全局目录条目:
[   60.123457] 页目录项 0: 0x123456789abcdef0
[   60.123458] 页目录项 1: 0x23456789abcdef01
...
```

##### 2.4 卸载模块

完成测试后，可以通过以下命令卸载模块：

```sh
sudo rmmod pgd_print
```

模块卸载时会打印一条消息确认卸载完成：

```plaintext
[   65.123456] 模块卸载完成
```

#### 3. 代码解析

##### 3.1 包含头文件

```c
#include <linux/kernel.h>      // 内核空间函数和宏定义
#include <linux/module.h>      // 内核模块支持
#include <linux/sched/task.h>  // 任务调度结构体定义
#include <linux/mm_types.h>    // 内存管理结构体定义
```

##### 3.2 初始化函数

```c
static int print_pgd_entries(void) {
    struct task_struct *ts = current;
    struct mm_struct *ms = ts->active_mm;
    pgd_t *pgd = ms->pgd;
    int i;

    printk(KERN_INFO "当前进程页全局目录条目:\n");
    
    for (i = 0; i < PTRS_PER_PGD; i++) {
        pgd_t entry = pgd[i];
        if (pgd_val(entry)) {
            printk(KERN_INFO "页目录项 %d: 0x%lx\n", i, pgd_val(entry));
        }
    }

    return 0;
}
```

- **获取当前进程的 `task_struct` 和 `mm_struct`**：使用 `current` 宏获取当前进程的任务结构指针，并从中提取内存管理结构指针。
- **遍历 PGD 条目**：遍历 PGD 并打印非空条目的虚拟地址。

##### 3.3 清理函数

```c
static void cleanup(void) {
    printk(KERN_INFO "模块卸载完成\n");
}
```

- **打印卸载信息**：在模块卸载时打印一条确认消息。

##### 3.4 模块宏定义

```c
module_init(print_pgd_entries);
module_exit(cleanup);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("打印当前进程的页全局目录条目");
```

- **指定初始化和清理函数**：使用 `module_init` 和 `module_exit` 宏来指定模块的初始化和清理函数。
- **模块元数据**：定义模块的许可证、描述等信息。


