
# **Print PDE Module**

## **简介**
`print_pde` 是一个 Linux 内核模块，用于打印当前进程的页目录表（PDE）信息。通过加载此模块，可以查看当前进程的页表基地址及其页目录表条目的详细信息，以用户友好的格式输出。

---

## **功能**
- 打印当前任务（进程）的页表基地址（PGD）。
- 遍历并打印页目录表（PDE）的有效条目。
- 输出每个条目的存在状态、读写权限和用户访问权限。

---

## **代码结构**
### **主要文件**
1. `print_pde.c`: 模块源代码，定义模块加载与卸载功能。
2. `Makefile`: 用于编译内核模块。
3. `README.md`: 文档说明。
```#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <asm/pgtable.h>
#include <asm/current.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Student");
MODULE_DESCRIPTION("Print current process page directory entries (PDE)");
MODULE_VERSION("1.0");

// 模块加载函数
static int __init print_pde_init(void) {
    struct task_struct *task = current; // 获取当前任务
    struct mm_struct *mm = task->mm;    // 获取当前任务的内存描述符
    pgd_t *pgd;                         // 页目录指针
    unsigned long address;              // 页表地址

    if (!mm) {
        printk(KERN_INFO "No memory descriptor found for the current process.\n");
        return -1;
    }

    pgd = mm->pgd; // 获取页表基地址
    printk(KERN_INFO "Page Directory Base Address: 0x%lx\n", (unsigned long)pgd);
    printk(KERN_INFO "PDEs for the current process:\n");

    for (address = 0; address < PTRS_PER_PGD; address++) {
        pgd_t entry = pgd[address];
        if (!pgd_none(entry)) { // 检查 PTE 是否为空
            printk(KERN_INFO "PDE[%lu]: 0x%lx (present: %d, write: %d, user: %d)\n",
                   address,
                   (unsigned long)pgd_val(entry),
                   !!(pgd_val(entry) & _PAGE_PRESENT),
                   !!(pgd_val(entry) & _PAGE_RW),
                   !!(pgd_val(entry) & _PAGE_USER));
        }
    }

    return 0;
}

// 模块卸载函数
static void __exit print_pde_exit(void) {
    printk(KERN_INFO "Exiting PDE printing module.\n");
}

module_init(print_pde_init);
module_exit(print_pde_exit);
```
---

## **编译与运行**

### **编译模块**
在模块源文件所在目录运行以下命令：
```bash
make
```
成功后会生成 `print_pde.ko` 模块文件。

### **加载模块**
加载模块以打印当前进程的页目录表信息：
```bash
sudo insmod print_pde.ko
```

### **查看输出**
使用以下命令查看模块的输出信息：
```bash
dmesg | tail -n 20
```

输出示例：
![image](https://github.com/n-i-c-e-ck/openeuler_lab/blob/main/lab8.png)
### **卸载模块**
运行以下命令卸载模块：
```bash
sudo rmmod print_pde
```

### **清理编译文件**
清理生成的编译文件：
```bash
make clean
```

---

## **实现细节**
1. **页表基地址**:
   - 使用 `task_struct` 的 `mm->pgd` 获取当前进程的页表基地址。
2. **页目录表（PDE）**:
   - 遍历 `pgd` 表项，检查有效条目并解析其值。
3. **条目属性**:
   - 解析存在位（`_PAGE_PRESENT`）、读写权限（`_PAGE_RW`）、用户权限（`_PAGE_USER`）。

---

