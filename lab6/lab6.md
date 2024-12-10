# 打印当前任务用户态/内核态堆栈起始位置

### （1）了解堆栈结构
1. **用户态堆栈**：
   用户态堆栈是进程运行时分配的栈空间，主要存储函数调用信息、局部变量等。其起始地址和大小由 `RLIMIT_STACK` 控制。

2. **内核态堆栈**：
   内核态堆栈是进程在内核模式下运行时使用的栈空间，通常固定大小（例如 x86-64 系统中为 8KB 或 16KB），其起始地址与当前栈指针对齐。

实验目标是通过系统接口打印当前任务的用户态和内核态堆栈起始地址以及堆栈大小。

---

### （2）编写 C 程序代码

逐行分析如下代码：
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>

// 定义内核栈大小（通常为 8KB）
#define KERNEL_STACK_SIZE (8 * 1024)

// 获取当前任务的用户态堆栈信息
void print_user_stack_info() {
    struct rlimit rlim;
    if (getrlimit(RLIMIT_STACK, &rlim) == 0) { // 获取用户栈的限制
        printf("User Stack:\n");
        printf("  Start Address: 0x%016lx\n", (unsigned long)&rlim); // 获取栈起始地址
        printf("  Stack Size: %lu KB\n", rlim.rlim_cur / 1024);     // 打印栈大小
    } else {
        perror("Failed to get user stack limit"); // 获取失败时打印错误
    }
}

// 获取当前任务的内核态堆栈信息
void print_kernel_stack_info() {
    void *stack_start;
    // 对当前栈指针进行对齐，计算栈的起始地址
    stack_start = (void *)((unsigned long)&stack_start & ~(KERNEL_STACK_SIZE - 1));
    printf("Kernel Stack:\n");
    printf("  Start Address: 0x%016lx\n", (unsigned long)stack_start); // 打印栈起始地址
    printf("  Stack Size: %d KB\n", KERNEL_STACK_SIZE / 1024);         // 打印栈大小
}

int main() {
    printf("Printing stack information for the current task:\n");
    print_user_stack_info();
    print_kernel_stack_info();
    return 0;
}
```

---

### （3）创建 Makefile 文档以编译程序
1. 在 `Makefile` 中设置编译器变量 `CC` 为 `gcc`，传递给编译器的选项变量 `CFLAGS` 为 `-Wall`。
2. 定义编译规则如下：
```makefile
CC=gcc
CFLAGS=-Wall

all: lab6

stack_info: lab6.c
	$(CC) $(CFLAGS) lab6.c -o lab6

clean:
	rm -f lab6
```

---

### （4）运行和测试
1. **编译程序**：在文件所在目录运行：
   ```bash
   make
   ```

2. **运行程序**：在终端中运行生成的程序：
   ```bash
   ./lab6
   ```

3. **输出示例**：
   成功运行后，程序将输出用户态和内核态堆栈的起始地址及大小：
  ![image](https://github.com/n-i-c-e-ck/openeuler_lab/blob/main/lab6.png)

4. **清理生成文件**：
   在终端中运行：
   ```bash
   make clean
   ```

---

