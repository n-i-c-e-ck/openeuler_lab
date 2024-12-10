！！！注意：由于选择x86架构，所以显示GDT表空
# 打印全局描述符表 (GDT) 信息

### （1）了解全局描述符表 (GDT)
1. **全局描述符表** (Global Descriptor Table, GDT) 是 x86 架构的核心数据结构之一，用于定义内存段的基地址、大小、权限等信息。
2. **GDTR 寄存器**：GDTR 是一个专用寄存器，保存 GDT 的基地址和长度。

实验的目标是通过访问 GDTR 寄存器，解析 GDT 表中的所有表项，并以用户友好的形式打印每个表项的基地址和段长。

### （2）编写 C 程序代码
逐行分析如下代码：
```c
#include <stdio.h>
#include <stdint.h>

// 定义 GDT 表项结构
typedef struct {
    uint16_t limit_low;  // 段界限低 16 位
    uint16_t base_low;   // 基地址低 16 位
    uint8_t  base_mid;   // 基地址中间 8 位
    uint8_t  access;     // 访问权限字节
    uint8_t  granularity; // 粒度与段界限高 4 位
    uint8_t  base_high;  // 基地址高 8 位
} __attribute__((packed)) gdt_entry_t;

// 定义 GDTR 寄存器结构
typedef struct {
    uint16_t limit;      // GDT 大小
    uint64_t base;       // GDT 基地址
} __attribute__((packed)) gdtr_t;

// 函数声明：从 GDTR 寄存器读取信息
void load_gdtr(gdtr_t *gdtr);

int main() {
    gdtr_t gdtr;
    load_gdtr(&gdtr); // 读取 GDTR 信息

    printf("GDT Base Address: 0x%016lx\n", gdtr.base);
    printf("GDT Limit: 0x%04x\n", gdtr.limit);

    // 解析并打印 GDT 表项信息
    gdt_entry_t *gdt_entries = (gdt_entry_t *)gdtr.base;
    int num_entries = (gdtr.limit + 1) / sizeof(gdt_entry_t);

    printf("Number of GDT Entries: %d\n", num_entries);
    printf("GDT Entries:\n");
    for (int i = 0; i < num_entries; i++) {
        gdt_entry_t entry = gdt_entries[i];
        uint32_t base = (entry.base_low | (entry.base_mid << 16) | (entry.base_high << 24));
        uint32_t limit = (entry.limit_low | ((entry.granularity & 0x0F) << 16));
        printf("Entry %d: Base=0x%08x, Limit=0x%05x\n", i, base, limit);
    }

    return 0;
}

// 内联汇编：读取 GDTR 寄存器
void load_gdtr(gdtr_t *gdtr) {
    asm volatile("sgdt %0" : "=m" (*gdtr));
}
```

### （3）创建 Makefile 文档以编译程序
1. 在 `Makefile` 中设置编译器变量 `CC` 为 `gcc`，传递给编译器的选项变量 `CFLAGS` 为 `-Wall`。
2. 定义编译规则如下：
```makefile
CC=gcc
CFLAGS=-Wall

all: gdt_info

gdt_info: gdt_info.c
	$(CC) $(CFLAGS) gdt_info.c -o gdt_info

clean:
	rm -f gdt_info
```

### （4）运行和测试
1. **编译程序**：在终端中输入 `make`。
2. **运行程序**：以 `root` 权限运行，输入：
   ```bash
   sudo ./gdt_info
   ```
3. **输出结果**：
   成功运行后，程序将输出 GDT 的基地址、长度以及所有表项的信息。示例输出如下：
![image](https://github.com/n-i-c-e-ck/openeuler_lab/blob/main/lab5.png)

4. **清理生成文件**：
   在终端中输入：
   ```bash
   make clean
   ```

---

