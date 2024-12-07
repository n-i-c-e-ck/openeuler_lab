#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_SIZE 8192                  // 内存池的总大小
#define BLOCK_SIZES_COUNT 7               // 支持的不同内存块大小的数量

// 定义支持的固定内存块大小
const size_t block_sizes[BLOCK_SIZES_COUNT] = {8, 16, 32, 64, 128, 256, 512};

// 内存块结构体，用于表示每个内存块的信息
typedef struct MemoryBlock {
    size_t size;                         // 内存块的大小
    int is_free;                         // 标记内存块是否空闲 (1: 空闲, 0: 已分配)
    struct MemoryBlock *next;            // 指向下一个内存块的指针
} MemoryBlock;

char memory[MEMORY_SIZE];                // 静态内存区域，模拟物理内存
MemoryBlock *free_list = NULL;           // 空闲块链表头指针

/**
 * @brief 初始化内存管理系统，设置初始空闲块
 */
void initialize_memory() {
    free_list = (MemoryBlock *)memory;
    free_list->size = MEMORY_SIZE - sizeof(MemoryBlock); // 减去头部信息占用的空间
    free_list->is_free = 1;                                // 设置为可用状态
    free_list->next = NULL;                                // 初始时没有其他块
    printf("内存初始化完成，大小: %lu 字节\n", MEMORY_SIZE);
}
size_t find_best_fit_size(size_t required_size) {
    for (int i = 0; i < BLOCK_SIZES_COUNT; i++) {
        if (block_sizes[i] >= required_size) return block_sizes[i];
    }
    return 0; // 如果找不到合适的大小，则返回0
}
void *allocate_memory(size_t size) {
    size_t block_size = find_best_fit_size(size);
    if (!block_size) {
        printf("错误: 没有足够大的块可以分配 %lu 字节\n", size);
        return NULL;
    }

    MemoryBlock *current = free_list;
    MemoryBlock *prev = NULL;

    while (current) {
        if (current->is_free && current->size >= block_size) {
            current->is_free = 0; // 将找到的块标记为已分配
            printf("分配了 %lu 字节，地址为 %p\n", block_size, (void *)((char *)current + sizeof(MemoryBlock)));
            return (void *)((char *)current + sizeof(MemoryBlock));
        }
        prev = current;
        current = current->next;
    }

    printf("错误: 没有足够的内存来分配 %lu 字节\n", size);
    return NULL;
}
void release_memory(void *ptr) {
    if (!ptr) {
        printf("错误: 空指针无法释放\n");
        return;
    }

    MemoryBlock *block = (MemoryBlock *)((char *)ptr - sizeof(MemoryBlock)); // 回退到块头部
    if (block->is_free) {
        printf("错误: 检测到重复释放，地址 %p\n", ptr);
        return;
    }

    block->is_free = 1; // 将块标记为空闲
    printf("释放了地址 %p 的内存，大小: %lu 字节\n", ptr, block->size);
}
void display_memory_status() {
    MemoryBlock *current = free_list;
    printf("内存状态:\n");
    while (current) {
        printf("  块地址 %p - 大小: %lu 字节, 状态: %s\n",
               (void *)current, current->size, current->is_free ? "空闲" : "已分配");
        current = current->next;
    }
    printf("\n");
}

// 主函数进行测试
int main() {
    initialize_memory(); // 初始化内存管理器
    display_memory_status(); // 显示初始化后的内存状态

    // 测试正常分配和释放
    void *ptr1 = allocate_memory(10);
    void *ptr2 = allocate_memory(50);
    display_memory_status(); // 显示分配后的内存状态

    release_memory(ptr1); // 释放第一次分配的内存
    display_memory_status(); // 显示释放后的内存状态

    void *ptr3 = allocate_memory(20); // 再次尝试分配20字节
    display_memory_status(); // 显示再次分配后的内存状态

    release_memory(ptr2); // 释放第二次分配的内存
    release_memory(ptr3); // 释放第三次分配的内存
    display_memory_status(); // 显示最终的内存状态

    // 测试异常情况
    printf("尝试分配超过内存池大小的块...\n");
    allocate_memory(MEMORY_SIZE + 1);

    printf("尝试释放空指针...\n");
    release_memory(NULL);

    printf("尝试重复释放同一块内存...\n");
    void *ptr4 = allocate_memory(8);
    release_memory(ptr4);
    release_memory(ptr4); // 应该检测到重复释放并报错

    // 测试大量小块分配与释放
    const int num_tests = 100;
    void *test_ptrs[num_tests];
    for (int i = 0; i < num_tests; ++i) {
        test_ptrs[i] = allocate_memory(8); // 分配8字节的小块
    }
    display_memory_status(); // 显示大量分配后的内存状态

    for (int i = 0; i < num_tests; ++i) {
        release_memory(test_ptrs[i]); // 依次释放所有分配的小块
    }
    display_memory_status(); // 显示全部释放后的内存状态

    // 测试大块分配与释放
    void *large_ptr = allocate_memory(2048); // 尝试分配较大的块
    display_memory_status();

    release_memory(large_ptr); // 释放大块
    display_memory_status();

    printf("所有测试完成。\n");
    return 0;
}
