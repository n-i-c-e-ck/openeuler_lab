#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <fcntl.h>
#include <unistd.h>

// 显示 ELF 文件头基本信息
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
