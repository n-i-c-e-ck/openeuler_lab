#### lab2：模拟 malloc/free 内存分配与回收
---

#### 1. 实验概述

本实验旨在实现一个简单的内存管理系统，该系统能够初始化一块固定的内存池，并提供基本的内存分配（`allocate_memory`）和释放（`release_memory`）功能。此外，还实现了对内存状态的显示功能（`display_memory_status`），以便于调试和验证。

#### 2. 代码解析

##### 2.1 包含头文件

```c
#include <stdio.h>      // 标准输入输出库，包含 printf 等函数
#include <stdlib.h>     // 标准库，包含一些常用的函数，如 atoi, exit 等
#include <string.h>     // 字符串操作库，包含 snprintf, strcspn 等函数
```

##### 2.2 定义常量和全局变量

```c
#define MEMORY_SIZE 8192                  // 内存池的总大小
#define BLOCK_SIZES_COUNT 7               // 支持的不同内存块大小的数量

const size_t block_sizes[BLOCK_SIZES_COUNT] = {8, 16, 32, 64, 128, 256, 512}; // 定义支持的固定内存块大小

char memory[MEMORY_SIZE];                // 静态内存区域，模拟物理内存
MemoryBlock *free_list = NULL;           // 空闲块链表头指针
```

##### 2.3 定义内存块结构体

```c
typedef struct MemoryBlock {
    size_t size;                         // 内存块的大小
    int is_free;                         // 标记内存块是否空闲 (1: 空闲, 0: 已分配)
    struct MemoryBlock *next;            // 指向下一个内存块的指针
} MemoryBlock;
```

##### 2.4 初始化内存管理系统

```c
void initialize_memory() {
    free_list = (MemoryBlock *)memory;
    free_list->size = MEMORY_SIZE - sizeof(MemoryBlock); // 减去头部信息占用的空间
    free_list->is_free = 1;                                // 设置为可用状态
    free_list->next = NULL;                                // 初始时没有其他块
    printf("内存初始化完成，大小: %lu 字节\n", MEMORY_SIZE);
}
```

##### 2.5 查找最合适的块大小

```c
size_t find_best_fit_size(size_t required_size) {
    for (int i = 0; i < BLOCK_SIZES_COUNT; i++) {
        if (block_sizes[i] >= required_size) return block_sizes[i];
    }
    return 0; // 如果找不到合适的大小，则返回0
}
```

##### 2.6 分配内存

```c
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
```

##### 2.7 释放内存

```c
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
```

##### 2.8 显示当前内存的状态

```c
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
```

#### 3. 主函数测试

主函数 `main` 中包含了多个测试用例，用于验证内存管理系统的正确性和稳定性：

- **常规测试**：进行基本的内存分配和释放操作。
- **异常情况测试**：尝试分配超出内存池大小的块、释放空指针或重复释放等。
- **批量测试**：通过循环进行大量的小块分配和释放，模拟更复杂的使用场景。
- **大块测试**：测试较大块内存的分配和释放。

#### 4. 编译与运行

为了方便编译和运行程序，我们创建了一个 `Makefile` 文件。以下是 `Makefile` 的内容：

```makefile
CC=gcc
CFLAGS=-Wall

all: lab1

lab1: lab1.c
	$(CC) $(CFLAGS) lab1.c -o lab1

clean:
	rm -f lab1
```

- 使用 `make` 命令编译程序。
- 运行编译后的程序，可以通过命令 `./lab1` 来执行。

![image](
