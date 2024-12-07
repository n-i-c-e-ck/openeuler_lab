#include <stdio.h>
#include <stdint.h>
// 这些是内核相关的头文件，在用户空间不可用。
#include <asm/desc_defs.h> // 用于 gdt_ptr 等 (x86 特定)
#include <asm/segment.h> // 用于 get_desc_base, get_desc_limit


void print_gdt(void) {
    struct desc_ptr gdt_ptr;
    // 这段代码具有特权，并且是体系结构相关的。
    sgdt(&gdt_ptr);

    printf("全局描述符表 (GDT) 信息:\n");
    unsigned long limit = gdt_ptr.limit; // GDT 限制 (大小)
    unsigned long base = gdt_ptr.address; // GDT 基地址

    printf("GDT 基地址: 0x%lx\n", base);
    printf("GDT 限制: 0x%lx\n", limit);

    // 遍历 GDT 条目。需要仔细计算大小
    for (unsigned int i = 0; i <= limit / sizeof(struct descriptor); ++i) {
        struct descriptor *desc = (struct descriptor *)(base + i * sizeof(struct descriptor));

        // 提取基地址和限制
        unsigned long base_addr = get_desc_base(desc); // 从描述符中提取基地址的函数
        unsigned long limit_val = get_desc_limit(desc); // 从描述符中提取限制的函数

        printf("条目 %d:\n", i);
        printf("  基地址: 0x%lx\n", base_addr);
        printf("  段限长: 0x%lx\n", limit_val);
        printf("\n");
    }
}

int init_module(void) { // 内核模块初始化函数 (仅用于内核模块)
    print_gdt();
    return 0;
}

void cleanup_module(void) { // 内核模块清理函数 (仅用于内核模块)
   // 此处无需执行任何操作
}
