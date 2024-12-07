#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

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
    unsigned int lba_sectors;    // 扇区数量
} partition_entry_t;


//分区类型查找表（概念性示例，需要替换为更全面的表）
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
        if (entry[i].type != 0x00) { //跳过空分区
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
