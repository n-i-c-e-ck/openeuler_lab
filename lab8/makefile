# 定义内核目录路径
KERNEL_SRC := /lib/modules/$(shell uname -r)/build

# 模块名称
MODULE_NAME := print_pde

# 默认目标
obj-m := $(MODULE_NAME).o

# 构建规则
all:
	make -C $(KERNEL_SRC) M=$(PWD) modules

# 清理规则
clean:
	make -C $(KERNEL_SRC) M=$(PWD) clean
	rm -rf *.o *.ko *.mod.c *.order *.symvers

