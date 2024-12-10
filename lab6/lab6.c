#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>

// 定义内核栈大小（通常在 x86-64 系统中为 8KB 或 16KB，视内核配置而定）
#define KERNEL_STACK_SIZE (8 * 1024)

// 获取当前任务的用户态堆栈起始地址
void print_user_stack_info() {
    struct rlimit rlim;
    if (getrlimit(RLIMIT_STACK, &rlim) == 0) {
        printf("User Stack:\n");
        printf("  Start Address: 0x%016lx\n", (unsigned long)&rlim);
        printf("  Stack Size: %lu KB\n", rlim.rlim_cur / 1024);
    } else {
        perror("Failed to get user stack limit");
    }
}

// 获取当前任务的内核态堆栈起始地址
void print_kernel_stack_info() {
    void *stack_start;
    stack_start = (void *)((unsigned long)&stack_start & ~(KERNEL_STACK_SIZE - 1));
    printf("Kernel Stack:\n");
    printf("  Start Address: 0x%016lx\n", (unsigned long)stack_start);
    printf("  Stack Size: %d KB\n", KERNEL_STACK_SIZE / 1024);
}

int main() {
    printf("Printing stack information for the current task:\n");
    print_user_stack_info();
    print_kernel_stack_info();
    return 0;
}
