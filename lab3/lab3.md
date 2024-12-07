
#### ELF 文件头信息读取实验

---

#### 1. 实验概述

本实验旨在实现一个简单的程序，用于读取并打印指定 ELF 文件的基本组成部分和长度信息。该程序将打开 ELF 文件，验证其有效性，并输出文件头的关键信息。

#### 2. 代码解析

##### 2.1 包含头文件

```c
#include <stdio.h>      // 标准输入输出库，包含 printf 等函数
#include <stdlib.h>     // 标准库，包含一些常用的函数，如 exit 等
#include <elf.h>        // ELF 文件格式定义库
#include <fcntl.h>      // 文件控制定义，如 open 函数的标志
#include <unistd.h>     // UNIX 标准库，包含 close 函数等
```

##### 2.2 显示 ELF 文件头基本信息

```c
void display_elf_header(const char *filename) {
    int fd;                        // 文件描述符
    Elf64_Ehdr ehdr;               // ELF 文件头结构

    // 打开文件
    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("打开文件失败");
        return;
    }

    // 读取文件头
    if (read(fd, &ehdr, sizeof(ehdr)) != sizeof(ehdr)) {
        perror("读取文件头失败");
        close(fd);
        return;
    }

    // 验证是否为有效的 ELF 文件
    if (ehdr.e_ident[EI_MAG0] != ELFMAG0 || 
        ehdr.e_ident[EI_MAG1] != ELFMAG1 || 
        ehdr.e_ident[EI_MAG2] != ELFMAG2 || 
        ehdr.e_ident[EI_MAG3] != ELFMAG3) {
        fprintf(stderr, "不是有效的 ELF 文件\n");
        close(fd);
        return;
    }

    // 输出 ELF 文件头的基本组成部分和长度
    printf("ELF Header:\n");
    printf(" - 类型: %s\n", ehdr.e_ident[EI_CLASS] == ELFCLASS32 ? "ELF32" : "ELF64");
    printf(" - 程序头表大小: %u 字节, 数量: %u\n", ehdr.e_phentsize, ehdr.e_phnum);
    printf(" - 段头表大小: %u 字节, 数量: %u\n", ehdr.e_shentsize, ehdr.e_shnum);

    // 关闭文件
    close(fd);
}
```

##### 2.3 主函数

```c
int main(int argc, char *argv[]) {
    // 检查命令行参数数量
    if (argc != 2) {
        fprintf(stderr, "用法: %s <ELF 文件>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // 将标准输出重定向到日志文件
    freopen("./task_10.log", "w", stdout);

    // 调用函数显示 ELF 文件头信息
    display_elf_header(argv[1]);

    return EXIT_SUCCESS;
}
```

#### 3. 编译与运行

为了方便编译和运行程序，我们创建了一个 `Makefile` 文件。以下是 `Makefile` 的内容：

```makefile
CC=gcc
CFLAGS=-Wall

all: elf_info

elf_info: elf_info.c
	$(CC) $(CFLAGS) elf_info.c -o elf_info

clean:
	rm -f elf_info
```

- 使用 `make` 命令编译程序。
- 运行编译后的程序，可以通过命令 `./elf_info <ELF 文件>` 来执行，并且输出会被重定向到 `task_10.log` 文件中。

#### 4. 测试用例

- **正常测试**：提供一个有效的 ELF 文件作为参数，检查输出的日志文件是否包含了正确的文件头信息。
- **异常测试**：尝试使用非 ELF 文件或不存在的文件作为参数，确保程序能够正确报告错误。


