#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched/task.h>
#include <linux/mm_types.h>

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

static void cleanup(void) {
    printk(KERN_INFO "模块卸载完成\n");
}

module_init(print_pgd_entries);
module_exit(cleanup);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("打印当前进程的页全局目录条目");
