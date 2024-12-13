#include <linux/init.h>
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
