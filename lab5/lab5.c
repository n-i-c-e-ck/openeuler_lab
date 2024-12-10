#include <stdio.h>
#include <stdint.h>

typedef struct {
    uint16_t limit_low;  // 段界限低 16 位
    uint16_t base_low;   // 基地址低 16 位
    uint8_t  base_mid;   // 基地址中间 8 位
    uint8_t  access;     // 访问权限字节
    uint8_t  granularity; // 粒度与段界限高 4 位
    uint8_t  base_high;  // 基地址高 8 位
} __attribute__((packed)) gdt_entry_t;

typedef struct {
    uint16_t limit;      // GDT 大小
    uint64_t base;       // GDT 基地址
} __attribute__((packed)) gdtr_t;

// 函数声明：从 gdtr 寄存器读取 GDTR 信息
void load_gdtr(gdtr_t *gdtr);

int main() {
    gdtr_t gdtr;
    load_gdtr(&gdtr); // 读取 GDTR 信息

    printf("GDT Base Address: 0x%016lx\n", gdtr.base);
    printf("GDT Limit: 0x%04x\n", gdtr.limit);

    // 遍历并打印 GDT 表项
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
