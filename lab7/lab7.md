以下是针对打印系统中所有进程名称实验的完整 README 文档：

---

# 打印系统所有进程名称

### （1）实验背景
在 Linux 系统中，每个进程都由内核中的 `task_struct` 结构表示。通过遍历内核的任务链表，可以获取系统中所有进程的信息，包括进程名称、PID 等。

本实验的目标是通过内核模块，打印当前系统中所有运行进程的名称。

---

### （2）实验代码

以下是 `list_processes.c` 的代码实现，核心功能是通过内核宏 `for_each_process` 遍历任务链表，并使用 `task_struct` 的 `comm` 成员打印进程名称。

```c
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
```

---

### （3）创建 Makefile

以下是 `Makefile` 的实现，用于编译上述代码生成内核模块。

```makefile
# 内核源码路径
KERNEL_DIR := /lib/modules/$(shell uname -r)/build

# 当前目录
PWD := $(shell pwd)

# 目标模块
obj-m := list_processes.o

# 默认目标
all:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules

# 清理生成文件
clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean
```

---

### （4）运行和测试

#### 1. **准备内核开发环境**
确保已安装内核开发工具和头文件：
```bash
sudo dnf install kernel-devel kernel-headers gcc make
```

#### 2. **编译模块**
在 `list_processes.c` 和 `Makefile` 所在目录运行以下命令：
```bash
make
```

成功后，生成的模块文件 `list_processes.ko` 应出现在当前目录。

#### 3. **加载模块**
加载模块以打印所有进程的名称：
```bash
sudo insmod list_processes.ko
```

#### 4. **查看输出**
使用 `dmesg` 查看模块输出的进程名称：
```bash
dmesg | tail -n 50
```

输出：
```
[ 1093.103504] Process: tpm_dev_wq
[ 1093.103504] Process: md
[ 1093.103505] Process: md_bitmap
[ 1093.103506] Process: edac-poller
[ 1093.103507] Process: watchdogd
[ 1093.103507] Process: kworker/1:1H
[ 1093.103508] Process: kswapd0
[ 1093.103509] Process: kthrotld
[ 1093.103510] Process: acpi_thermal_pm
[ 1093.103511] Process: kmpath_rdacd
[ 1093.103511] Process: kaluad
[ 1093.103512] Process: ipv6_addrconf
[ 1093.103513] Process: kworker/0:1H
[ 1093.103514] Process: kstrp
[ 1093.103514] Process: kworker/u5:0
[ 1093.103515] Process: kworker/1:2
[ 1093.103516] Process: kworker/1:3
[ 1093.103517] Process: kworker/0:5
[ 1093.103517] Process: scsi_eh_0
[ 1093.103518] Process: scsi_tmf_0
[ 1093.103519] Process: ata_sff
[ 1093.103520] Process: scsi_eh_1
[ 1093.103520] Process: scsi_tmf_1
[ 1093.103521] Process: scsi_eh_2
[ 1093.103522] Process: scsi_tmf_2
[ 1093.103523] Process: jbd2/sda2-8
[ 1093.103523] Process: ext4-rsv-conver
[ 1093.103524] Process: systemd-journal
[ 1093.103525] Process: systemd-udevd
[ 1093.103526] Process: nfit
[ 1093.103527] Process: auditd
[ 1093.103527] Process: dbus-daemon
[ 1093.103528] Process: systemd-logind
[ 1093.103529] Process: NetworkManager
[ 1093.103529] Process: dhclient
[ 1093.103530] Process: rsyslogd
[ 1093.103531] Process: sshd
[ 1093.103531] Process: crond
[ 1093.103532] Process: agetty
[ 1093.103533] Process: agetty
[ 1093.103534] Process: sshd
[ 1093.103534] Process: sshd
[ 1093.103535] Process: sftp-server
[ 1093.103535] Process: bash
[ 1093.103536] Process: kworker/u4:1
[ 1093.103537] Process: kworker/1:0
[ 1093.103537] Process: kworker/0:0
[ 1093.103538] Process: kworker/u4:2
[ 1093.103538] Process: sudo
[ 1093.103538] Process: insmod
```
![image](https://github.com/n-i-c-e-ck/openeuler_lab/blob/main/lab7.png)
#### 5. **卸载模块**
完成后卸载模块：
```bash
sudo rmmod list_processes
```

#### 6. **清理生成文件**
清理编译生成的临时文件：
```bash
make clean
```

---

