
#### MBR 分区表读取工具



#### 1. 使用方法

##### 1.1 编译

确保你已经安装了 C 编译器（如 GCC）。使用以下命令编译程序：

```sh
gcc -o mbr_reader mbr_reader.c
```

##### 1.2 运行

编译完成后，可以通过以下命令运行程序，并提供一个磁盘映像文件作为参数：

```sh
./mbr_reader <磁盘映像>
```

例如，如果你有一个名为 `disk.img` 的磁盘映像文件，可以使用以下命令来读取并显示其 MBR 分区表：

```sh
./mbr_reader disk.img
```

##### 1.3 输出示例

假设磁盘映像文件中有两个分区，运行程序后可能看到如下输出：

```plaintext
MBR 分区表:
分区 1:
  状态: 活动
  类型: 0x83 (Linux)
  起始 LBA: 2048
  扇区数: 1048576
  大小: 512.00 MB

分区 2:
  状态: 非活动
  类型: 0x07 (NTFS)
  起始 LBA: 1050624
  扇区数: 2097152
  大小: 1024.00 MB
```
# 创建和初始化磁盘映像 (`disk.img`)

## 1. 创建空白磁盘映像文件
使用 `dd` 命令创建一个大小为 1MB 的空白磁盘映像：
```bash
dd if=/dev/zero of=disk.img bs=512 count=2048
```
###2.初始化
```bash
fdisk disk.img
o
n
enter
enter
```
![image](https://github.com/n-i-c-e-ck/openeuler_lab/blob/main/AF917CCB7CEC302383859FC25F9AA41F.png)
#### 2. 代码解析

##### 2.1 包含头文件

```c
#include <stdio.h>      // 标准输入输出库，包含 printf 等函数
#include <stdlib.h>     // 标准库，包含一些常用的函数，如 exit 等
#include <stdint.h>     // 固定宽度整数类型
#include <fcntl.h>      // 文件控制定义，如 open 函数的标志
#include <unistd.h>     // UNIX 标准库，包含 close 函数等
#include <string.h>     // 字符串处理函数，如 memcpy 等
```

##### 2.2 MBR 结构体定义

```c
// MBR 结构体
typedef struct {
    unsigned char boot_code[446]; // 引导程序代码
    unsigned char partition_table[64]; // 分区表 (4 个条目)
    unsigned char signature[2]; // 签名 (0x55, 0xAA)
} mbr_t;

// 分区条目结构体
typedef struct {
    unsigned char status;         // 0x00 = 非活动，0x80 = 活动
    unsigned char chs_start[3];   // 起始 CHS 地址 (柱面、磁头、扇区)
    unsigned char type;           // 分区类型
    unsigned char chs_end[3];     // 结束 CHS 地址 (柱面、磁头、扇区)
    unsigned int lba_start;       // 起始 LBA 地址 (逻辑块地址)
    unsigned int lba_sectors;     // 扇区数量
} partition_entry_t;
```

##### 2.3 分区类型查找表

```c
// 分区类型查找表（概念性示例，需要替换为更全面的表）
const char *get_partition_type(unsigned char type) {
    switch (type) {
        case 0x00: return "空";
        case 0x01: return "FAT12";
        case 0x04: return "FAT16 (<32M)";
        case 0x05: return "扩展分区";
        case 0x06: return "FAT16";
        case 0x07: return "NTFS";
        case 0x0b: return "FAT32";
        case 0x83: return "Linux";
        case 0x82: return "Linux 交换分区";
        default: return "未知";
    }
}
```

##### 2.4 主函数

```c
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "使用方法: %s <磁盘映像>\n", argv[0]);
        return 1;
    }

    // 打开磁盘映像文件
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("打开磁盘映像失败");
        return 1;
    }

    // 读取 MBR
    mbr_t mbr;
    ssize_t bytes_read = read(fd, &mbr, sizeof(mbr));
    if (bytes_read != sizeof(mbr)) {
        perror("读取 MBR 失败");
        close(fd);
        return 1;
    }

    // 检查签名
    if (mbr.signature[0] != 0x55 || mbr.signature[1] != 0xAA) {
        fprintf(stderr, "无效的 MBR 签名\n");
        close(fd);
        return 1;
    }

    printf("MBR 分区表:\n");
    partition_entry_t *entry = (partition_entry_t *)mbr.partition_table;
    for (int i = 0; i < 4; i++) {
        if (entry[i].type != 0x00) { // 跳过空分区
            printf("分区 %d:\n", i + 1);
            printf("  状态: %s\n", (entry[i].status == 0x80) ? "活动" : "非活动");
            printf("  类型: 0x%02X (%s)\n", entry[i].type, get_partition_type(entry[i].type));
            printf("  起始 LBA: %u\n", entry[i].lba_start);
            printf("  扇区数: %u\n", entry[i].lba_sectors);
            unsigned long long size_bytes = (unsigned long long)entry[i].lba_sectors * 512;
            printf("  大小: %.2f MB\n", (double)size_bytes / (1024 * 1024));
            printf("\n");
        }
    }

    close(fd);
    return 0;
}
```

